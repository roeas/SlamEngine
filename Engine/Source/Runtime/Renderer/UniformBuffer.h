#pragma once

#include "Renderer/Layout.h"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace sl
{

class UniformBuffer
{
public:
    static UniformBuffer *Create(uint32_t bindignPoint, UniformBufferLayout layout);

public:
    virtual ~UniformBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void Upload(StringHashType name, const void *pData, uint32_t size = 0) const = 0;
};

} // namespace sl
