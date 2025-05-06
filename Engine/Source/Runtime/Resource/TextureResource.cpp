#include "TextureResource.h"

#include "Core/Log.h"
#include "Renderer/Texture.h"
#include "Utils/FileIO.hpp"
#include "Utils/ProfilerCPU.h"

#include <magic_enum/magic_enum.hpp>
#include <stb/stb_image.h>

namespace sl
{

TextureResource::TextureResource(std::string assetPath, bool mipmap, uint32_t flags) :
    m_assetPath(std::move(assetPath)), m_width(0), m_height(0), m_channels(0),
    m_format(TextureFormat::RGB8), m_isHDR(false), m_mipmap(mipmap), m_flags(flags)
{

}

TextureResource::~TextureResource()
{
    OnDestroy();
}

void TextureResource::OnImport()
{
    SL_PROFILE;
    SL_LOG_TRACE("Importing image \"{}\"", m_assetPath.data());

    // The first pixel should at the bottom left
    stbi_set_flip_vertically_on_load(true);

    void *pImageData;
    int width, height, channel;
    auto assetData = FileIO::ReadBinary(m_assetPath.data());
    bool isHDR = stbi_is_hdr_from_memory((stbi_uc *)assetData.data(), (int)assetData.size());
    bool getInfoSuccess = stbi_info_from_memory((stbi_uc *)assetData.data(), (int)assetData.size(), &width, &height, &channel);

    // NO support for 1 / 2 channel texture for now
    int desiredChannels = 0;
    if (channel == 1)
    {
        SL_LOG_TRACE("\tExpand 'Grey' to 'RGB'");
        desiredChannels = 3;
    }
    else if (channel == 2)
    {
        SL_LOG_TRACE("\tExpand 'GreyAlpha' to 'RGBA'");
        desiredChannels = 4;
    }

    // HDR
    if (isHDR)
    {
        pImageData = stbi_loadf_from_memory((stbi_uc *)assetData.data(), (int)assetData.size(),
            &width, &height, &channel, desiredChannels);
    }
    else
    {
        pImageData = stbi_load_from_memory((stbi_uc *)assetData.data(), (int)assetData.size(),
            &width, &height, &channel, desiredChannels);
    }

    if (!pImageData || !getInfoSuccess || width <= 0 || height <= 0 || channel <= 0)
    {
        SL_LOG_ERROR("Invalid image: {}", stbi_failure_reason());
        m_state = ResourceState::Destroying;
        return;
    }

    m_width = (uint32_t)width;
    m_height = (uint32_t)height;
    m_channels = desiredChannels ? (uint8_t)desiredChannels : (uint8_t)channel;
    m_isHDR = isHDR;
    switch (m_channels)
    {
        case 1:
        {
            m_format = m_isHDR ? sl::TextureFormat::R32F : sl::TextureFormat::R8;
            break;
        }
        case 2:
        {
            m_format = m_isHDR ? sl::TextureFormat::RG32F : sl::TextureFormat::RG8;
            break;
        }
        case 3:
        {
            m_format = m_isHDR ? sl::TextureFormat::RGB32F : sl::TextureFormat::RGB8;
            break;
        }
        case 4:
        {
            m_format = m_isHDR ? sl::TextureFormat::RGBA32F : sl::TextureFormat::RGBA8;
            break;
        }
    }

    SL_LOG_TRACE("\tWidth: {}, Height: {}, Channels: {}, Format: {}, IsHDR: {}",
        m_width, m_height, m_channels, magic_enum::enum_name(m_format).data(), m_isHDR);

    m_imageData.resize(m_width * m_height * m_channels * (m_isHDR ? 4 : 1));
    memcpy(m_imageData.data(), pImageData, m_imageData.size());
    stbi_image_free(pImageData);

    m_state = ResourceState::Building;
}

void TextureResource::OnBuild()
{
    m_state = ResourceState::Uploading;
}

void TextureResource::OnLoad()
{
    m_state = ResourceState::Uploading;
}

void TextureResource::OnUpload()
{
    SL_PROFILE;

    m_pTexture.reset(Texture2D::Create(m_width, m_height, m_format, m_mipmap, m_flags, m_imageData.data()));
    m_state = ResourceState::Ready;
}

void TextureResource::OnReady()
{
    if (!m_optimized && !m_destroyDelayFrame--) [[unlikely]]
    {
        DestroyCPUData();
    }
}

void TextureResource::OnDestroy()
{
    DestroyCPUData();
    m_pTexture.reset();
    m_state = ResourceState::Destroyed;
}

void TextureResource::DestroyCPUData()
{
    m_imageData.clear();
    std::vector<unsigned char>{}.swap(m_imageData);

    m_optimized = true;
}

} //namespace sl
