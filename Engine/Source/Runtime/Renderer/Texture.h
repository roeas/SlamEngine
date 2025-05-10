#pragma once

#include "Core/Defines.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sl
{

// TODO: Wrap texture parameters into a common structure

class Texture
{
public:
    virtual ~Texture() = default;

    virtual void Bind(uint32_t slot) const = 0;
    virtual uint32_t GetHandle() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetDepth() const = 0;
    virtual TextureFormat GetFormat() const = 0;
    virtual uint32_t GetFlags() const = 0;
};

class Texture2D : public Texture
{
public:
    static Texture2D *Create(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, uint32_t flags, const void *pData = nullptr);

public:
    virtual void SetBorderColor(const glm::vec4 &color) const = 0;
    virtual void Clear(const void *pClearData) const = 0;
    virtual void Resize(uint32_t width, uint32_t height, const void *pData = nullptr) = 0;
};

class Texture3D
{
    // TODO
};

class TextureCube : public Texture
{
public:
    static TextureCube *Create(uint32_t width, uint32_t height, uint32_t mipmapCount, TextureFormat format, bool genMipmap, uint32_t flags, const void *pData = nullptr);
};

} // namespace sl
