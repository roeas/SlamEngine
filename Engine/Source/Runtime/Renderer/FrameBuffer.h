#pragma once

#include "Renderer/Texture.h"

#include <cstdint>
#include <initializer_list>

namespace sl
{

class Framebuffer
{
public:
    static Framebuffer *Create(std::initializer_list<Texture2D *> pTextures, bool destroy = true);

public:
    virtual ~Framebuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual void Clear(uint32_t attachmentIndex, const void *pClearData) const = 0;
    virtual int ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y) = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetAttachmentHandle(size_t attachmentIndex) const = 0;
};

} // namespace sl
