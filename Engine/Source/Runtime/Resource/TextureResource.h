#pragma once

#include "Core/Defines.h"
#include "Resource/Resource.h"

#include <memory>
#include <string>
#include <vector>

namespace sl
{

class Texture2D;

class TextureResource : public Resource
{
public:
    TextureResource(std::string_view sourcePath, bool mipmap, uint32_t flags = 0);
    TextureResource(const TextureResource &) = delete;
    TextureResource &operator=(const TextureResource &) = delete;
    TextureResource(TextureResource &&) = delete;
    TextureResource &operator=(TextureResource &&) = delete;
    ~TextureResource() override;

    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    Texture2D *GetTexture() const { return m_pTexture.get(); }

    std::string m_assetPath;
    std::vector<uint8_t> m_rawData;

    uint32_t m_flags = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
    uint8_t m_channels = 0;
    bool m_isHDR = false;
    bool m_mipmap = false;
    sl::TextureFormat m_format;

    std::unique_ptr<Texture2D> m_pTexture;
};

} // namespace sl
