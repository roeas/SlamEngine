#include "TextureResource.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Renderer/Texture.h"
#include "Utils/FileIO.hpp"
#include "Utils/ProfilerCPU.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <gli/gli.hpp>
#include <magic_enum/magic_enum.hpp>
#include <stb/stb_image.h>

// TODO: Refactor these shit

namespace sl
{

TextureResource::TextureResource(std::string assetPath, bool mipmap, uint32_t flags) :
    m_assetPath(std::move(assetPath)), m_width(0), m_height(0), m_mipmapCount(1),
    m_flags(flags), m_format(TextureFormat::RGBA8), m_mipmap(mipmap), m_isCubemap(false)
{
    m_name = Path::NameWithoutExtension(m_assetPath);
}

TextureResource::~TextureResource()
{
    OnDestroy();
}

void TextureResource::OnImport()
{
    SL_PROFILE;
    SL_LOG_TRACE("Importing image \"{}\"", m_assetPath.data());

    std::string extension = Path::Extension(m_assetPath);
    m_isCubemap = extension == ".ktx" || extension == ".dds";
    std::vector<unsigned char> assetData = FileIO::ReadBinary(m_assetPath.data());

    if(m_isCubemap) // Use GLI to import KTX and DDS
    {
        /*
         * We pre-convolve skybox by cmft and store it as ktx or dds,
         * texture format is hardcoded here for now.
         * See "SlamEngine/Tool/cmft/cmft.bat" for details.
         * 
         * TODO: Support KTX2
         */

        gli::texture texture = gli::load((char const *)assetData.data(), assetData.size());
        if (texture.empty() || texture.target() != gli::TARGET_CUBE || texture.format() != gli::FORMAT_RGBA32_SFLOAT_PACK32)
        {
            SL_LOG_ERROR("Invalid image.");
            m_state = ResourceState::Destroying;
            return;
        }

        size_t layer = texture.layers();
        size_t face = texture.faces();
        size_t level = texture.levels();
        size_t textureSize = texture.size();
        size_t faceSize = textureSize / face;
        auto extent = texture.extent();
        SL_ASSERT(layer == 1 && face == 6);

        m_width = extent.x;
        m_height = extent.y;
        m_mipmapCount = (uint32_t)level;
        m_format = TextureFormat::RGBA32F;

        SL_LOG_TRACE("\tWidth: {}, Height: {}, Mipmap: {}, Channels: {}, Format: {}, IsHDR: {}",
            m_width, m_height, m_mipmapCount, 4, "RGBA32F", true);

        m_imageData.resize(textureSize);
        for (size_t f = 0; f < face; ++f)
        {
            unsigned char *pOffset = m_imageData.data() + f * faceSize;
            for (size_t l = 0; l < level; ++l)
            {
                size_t levelSize = texture.size(l);
                memcpy(pOffset, texture.data(0, f, l), levelSize);
                pOffset += levelSize;
            }
        }
    }
    else // Use stb__image to import normal format
    {
        // The first pixel should at the bottom left
        stbi_set_flip_vertically_on_load(true);

        void *pImageData;
        int width, height, channel;
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
        uint8_t channels = desiredChannels ? (uint8_t)desiredChannels : (uint8_t)channel;
        switch (channels)
        {
            case 1:
            {
                m_format = isHDR ? sl::TextureFormat::R32F : sl::TextureFormat::R8;
                break;
            }
            case 2:
            {
                m_format = isHDR ? sl::TextureFormat::RG32F : sl::TextureFormat::RG8;
                break;
            }
            case 3:
            {
                m_format = isHDR ? sl::TextureFormat::RGB32F : sl::TextureFormat::RGB8;
                break;
            }
            case 4:
            {
                m_format = isHDR ? sl::TextureFormat::RGBA32F : sl::TextureFormat::RGBA8;
                break;
            }
        }

        SL_LOG_TRACE("\tWidth: {}, Height: {}, Channels: {}, Format: {}, IsHDR: {}",
            m_width, m_height, channels, magic_enum::enum_name(m_format).data(), isHDR);

        m_imageData.resize(m_width * m_height * channels * (isHDR ? 4 : 1));
        memcpy(m_imageData.data(), pImageData, m_imageData.size());
        stbi_image_free(pImageData);
    }

    m_state = ResourceState::Building;
}

void TextureResource::OnBuild()
{
    // TODO: Compression
    m_state = ResourceState::Uploading;
}

void TextureResource::OnLoad()
{
    m_state = ResourceState::Uploading;
}

void TextureResource::OnUpload()
{
    SL_PROFILE;

    if (m_isCubemap)
    {
        m_pTexture.reset(TextureCube::Create(m_width, m_height, m_mipmapCount, m_format, m_mipmap, m_flags, m_imageData.data()));
    }
    else
    {
        m_pTexture.reset(Texture2D::Create(m_width, m_height, m_format, m_mipmap, m_flags, m_imageData.data()));
    }
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
