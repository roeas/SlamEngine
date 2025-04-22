#include "TextureResource.h"

#include "Core/Log.h"
#include "Utils/FileIO.hpp"

#include <stb/stb_image.h>

namespace sl
{

TextureResource::TextureResource(std::string_view sourcePath, bool mipmap, uint32_t flags) :
    m_sourcePath(sourcePath), m_flags(flags), m_mipmap(mipmap), m_format(TextureFormat::RGB8)
{

}

TextureResource::~TextureResource()
{
    OnDestroy();
}

void TextureResource::OnImport()
{
    SL_LOG_TRACE("Loading image {}", m_sourcePath.data());
    auto optOriginalData = FileIO::ReadBinary(m_sourcePath);
    if (!optOriginalData)
    {
        SL_LOG_ERROR("Failed to load image {}", m_sourcePath.data());
        m_state = ResourceState::Destroying;
        return;
    }

    // The first pixel should at the bottom left
    stbi_set_flip_vertically_on_load(true);

    void *pTextureData;
    int width, height, channel;
    bool isHDR = stbi_is_hdr_from_memory((stbi_uc *)optOriginalData->data(), (int)optOriginalData->size());
    bool getInfoSuccess = stbi_info_from_memory((stbi_uc *)optOriginalData->data(), (int)optOriginalData->size(), &width, &height, &channel);

    int desiredChannels = 0;
    if (channel == 1)
    {
        desiredChannels = 3;
        SL_LOG_TRACE("Expand 'Grey' to 'RGB'");
    }
    else if (channel == 2)
    {
        desiredChannels = 4;
        SL_LOG_TRACE("Expand 'GreyAlpha' to 'RGBA'");
    }

    if (isHDR)
    {
        pTextureData = stbi_loadf_from_memory((stbi_uc *)optOriginalData->data(), (int)optOriginalData->size(),
            &width, &height, &channel, desiredChannels);
    }
    else
    {
        pTextureData = stbi_load_from_memory((stbi_uc *)optOriginalData->data(), (int)optOriginalData->size(),
            &width, &height, &channel, desiredChannels);
    }

    if (!pTextureData || !getInfoSuccess || width <= 0 || height <= 0 || channel <= 0)
    {
        SL_LOG_ERROR("Invalid image {}: {}", m_sourcePath.c_str(), stbi_failure_reason());
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
            SL_LOG_ERROR("Unknown texture format {}", m_sourcePath.c_str());
            m_state = ResourceState::Destroying;
        }
    }

    SL_LOG_TRACE("Width: {}, Height: {}, Channels: {}, Format: {}, IsHDR: {}",
        m_width, m_height, m_channels, nameof::nameof_enum<>(m_format), m_isHDR);

    m_rawData.resize(m_width * m_height * m_channels * (m_isHDR ? 4 : 1));
    memcpy(m_rawData.data(), pTextureData, m_rawData.size());
    stbi_image_free(pTextureData);

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
    m_pTexture.reset(Texture2D::Create(m_width, m_height, m_format, m_mipmap, m_flags, m_rawData.data()));
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
    m_optimized = true;
    m_rawData.clear();
    std::vector<uint8_t>{}.swap(m_rawData);
}

} //namespace sl
