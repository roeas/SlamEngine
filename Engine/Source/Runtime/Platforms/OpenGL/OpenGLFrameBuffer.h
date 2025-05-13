#pragma once

#include "Renderer/Framebuffer.h"

#include <vector>

namespace sl
{

struct Attachment
{
    Texture2D *m_pTexture;
    uint32_t m_point;
};

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(std::initializer_list<Texture2D *> pTextures, bool destroy = false);
    ~OpenGLFramebuffer() override;

    void Bind() const override;
    void Unbind() const override;

    void Resize(uint32_t width, uint32_t height) override;
    void Clear(uint32_t attachmentIndex, const void *pClearData) const override;
    int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) override;

    uint32_t GetWidth() const override { return m_width; }
    uint32_t GetHeight() const override { return m_height; }
    Texture2D *GetAttachment(size_t attachmentIndex) const override;

private:
    void Create();

    std::vector<Attachment> m_attachments;
    uint32_t m_handle;
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_colorAttachmentCount;
    bool m_destroyTextures;
};

} // namespace sl
