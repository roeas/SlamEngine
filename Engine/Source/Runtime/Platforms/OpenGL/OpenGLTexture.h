#pragma once

#include "Renderer/Texture.h"

namespace sl
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, bool genMipmap, uint32_t flags, const void *pData = nullptr);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot) const override;
    uint32_t GetHandle() const override { return m_handle; }
    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    uint32_t GetDepth() const override { return 0; }
    TextureFormat GetFormat() const override { return m_format; }
    uint32_t GetFlags() const override { return m_flags; }

    void SetBorderColor(const glm::vec4 &color) const override;
    void Clear(const void *pClearData) const override;
    void Resize(uint32_t width, uint32_t height, const void *pData = nullptr) override;

private:
    void Create(const void *pData = nullptr);

    uint32_t m_handle;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_flags;
    TextureFormat m_format;
    bool m_genMipmap;
};

class OpenGLTextureCube : public TextureCube
{
public:
    OpenGLTextureCube(uint32_t width, uint32_t height, TextureFormat format, bool genMipmap, uint32_t flags, std::vector<std::vector<const void *>> pDatas);
    ~OpenGLTextureCube() override;

    void Bind(uint32_t slot) const override;
    uint32_t GetHandle() const override { return m_handle; }
    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    uint32_t GetDepth() const override { return 6; }
    TextureFormat GetFormat() const override { return m_format; }
    uint32_t GetFlags() const override { return m_flags; }

private:
    void Create(std::vector<std::vector<const void *>> pDatas);

    uint32_t m_handle;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_flags;
    TextureFormat m_format;
    bool m_genMipmap;
};

} // namespace sl
