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
    TextureResource(std::string assetPath, bool mipmap, uint32_t flags = 0);
    TextureResource(const TextureResource &) = delete;
    TextureResource &operator=(const TextureResource &) = delete;
    TextureResource(TextureResource &&) = delete;
    TextureResource &operator=(TextureResource &&) = delete;
    ~TextureResource() override;

    void SetName(std::string name) { m_name = std::move(name); }
    const std::string &GetName() const { return m_name; }

    Texture2D *GetTexture() const { return m_pTexture.get(); }

private:
    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    std::string m_name;
    std::string m_assetPath;
    std::vector<unsigned char> m_imageData;

    uint32_t m_width;
    uint32_t m_height;
    uint8_t m_channels;
    sl::TextureFormat m_format;
    bool m_isHDR;
    bool m_mipmap;
    uint32_t m_flags;

    std::unique_ptr<Texture2D> m_pTexture;
};

} // namespace sl
