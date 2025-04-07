#pragma once

#include "Core/Defines.h"
#include "Renderer/Texture.h"

namespace sl
{

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, uint32_t flags, const void *pData = nullptr);
    ~OpenGLTexture2D() override;

    void Bind(uint32_t slot) const override;

    void SetBorderColor(const glm::vec4 &color) const override;
    void Clear(const void *pClearData) const override;
    void Resize(uint32_t width, uint32_t height, const void *pData = nullptr) override;

    uint32_t GetHandle() const override { return m_handle; }
    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }

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
