#pragma once

#include "Core/Defines.h"
#include "Renderer/Texture.h"
#include "Resource/Resource.h"

#include <memory>
#include <string>
#include <vector>

namespace sl
{

class TextureResource : public Resource
{
public:
    TextureResource(std::string assetPath, bool mipmap, uint32_t flags = 0);
    TextureResource(const TextureResource &) = delete;
    TextureResource &operator=(const TextureResource &) = delete;
    TextureResource(TextureResource &&) = delete;
    TextureResource &operator=(TextureResource &&) = delete;
    ~TextureResource() override;

    Texture *GetTexture() const { return m_pTexture.get(); }

private:
    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    std::string m_assetPath;
    std::vector<unsigned char> m_imageData;
    std::unique_ptr<Texture> m_pTexture;

    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_mipmapCount;
    uint32_t m_flags;
    sl::TextureFormat m_format;
    bool m_mipmap;
    bool m_isCubemap;
};

} // namespace sl
