#pragma once

#include <glm/vec4.hpp>

namespace sl
{

class RenderAPI
{
public:
    static RenderAPI *Create();

public:
    virtual ~RenderAPI() = default;

    virtual void SetClearColor(const glm::vec4 &color) = 0;
    virtual void SetClearDepth(float depth) = 0;
    virtual void SetClearStencil(int stencil) = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
