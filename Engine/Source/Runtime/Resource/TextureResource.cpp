#include "TextureResource.h"

#include "Core/Log.h"
#include "Renderer/Texture.h"
#include "Utils/FileIO.hpp"

#include <stb/stb_image.h>

namespace sl
{

TextureResource::TextureResource(std::string sourcePath, bool mipmap, uint32_t flags) :
    Resource(std::move(sourcePath), ""), m_width(0), m_height(0), m_channels(0),
    m_format(TextureFormat::RGB8), m_isHDR(false), m_mipmap(mipmap), m_flags(flags)
{

}

TextureResource::~TextureResource()
{
    OnDestroy();
}

void TextureResource::OnImport()
{
    SL_LOG_TRACE("Loading image \"{}\"", m_assettPath.data());
    auto assetData = FileIO::ReadBinary(m_assettPath);

    // The first pixel should at the bottom left
    stbi_set_flip_vertically_on_load(true);

    void *pImageData;
    int width, height, channel;
    bool isHDR = stbi_is_hdr_from_memory((stbi_uc *)assetData.data(), (int)assetData.size());
    bool getInfoSuccess = stbi_info_from_memory((stbi_uc *)assetData.data(), (int)assetData.size(), &width, &height, &channel);

    // Do not support 1 / 2 channel image for now
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
    m_channels = desiredChannels ? (uint32_t)desiredChannels : (uint32_t)channel;
    m_isHDR = isHDR;
    switch (m_channels)
    {
        case 1:
        {
            if (m_isHDR)
            {
                m_format = sl::TextureFormat::R32F;
            }
            else
            {
                m_format = sl::TextureFormat::R8;
            }
            break;
        }
        case 2:
        {
            if (m_isHDR)
            {
                m_format = sl::TextureFormat::RG32F;
            }
            else
            {
                m_format = sl::TextureFormat::RG8;
            }
            break;
        }
        case 3:
        {
            if (m_isHDR)
            {
                m_format = sl::TextureFormat::RGB32F;
            }
            else
            {
                m_format = sl::TextureFormat::RGB8;
            }
            break;
        }
        case 4:
        {
            if (m_isHDR)
            {
                m_format = sl::TextureFormat::RGBA32F;
            }
            else
            {
                m_format = sl::TextureFormat::RGBA8;
            }
            break;
        }
        default:
        {
            SL_LOG_ERROR("Unknown texture format!");
            m_state = ResourceState::Destroying;
        }
    }

    SL_LOG_TRACE("\tWidth: {}, Height: {}, Channels: {}, Format: {}, IsHDR: {}",
        m_width, m_height, m_channels, nameof::nameof_enum<>(m_format), m_isHDR);

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
    m_pTexture.reset(Texture2D::Create(m_width, m_height, m_format, m_mipmap, m_flags, m_imageData.data()));
    m_state = ResourceState::Ready;
}

void TextureResource::OnReady()
{
    if (!m_optimized) [[unlikely]]
    {
        if (!m_destroyDelayFrame--)
        {
            DestroyCPUData();
        }
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
    std::vector<uint8_t>{}.swap(m_imageData);

    m_optimized = true;
}

} //namespace sl
