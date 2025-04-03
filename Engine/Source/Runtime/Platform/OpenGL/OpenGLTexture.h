#pragma once

#include "Core/Defines.h"
#include "Renderer/Texture.h"

namespace sl
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags = 0, const void *pData = nullptr);
    ~OpenGLTexture2D() override;

    uint32_t GetHandle() const override { return m_handle; }
    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    TextureFormat GetFormat() const  override { return m_format; }

    void Clear(const void *pClearData) const override;
    void Resize(uint32_t width, uint32_t height, const void *pData = nullptr) override;

    void Bind(uint32_t slot) const override;

private:
    void Create(const void *pData = nullptr);

    uint32_t m_handle;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_flags;
    TextureFormat m_format;
    bool m_hasMip;
};

} // namespace sl
