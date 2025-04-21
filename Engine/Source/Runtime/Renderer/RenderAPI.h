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

    virtual void ClearColor(const glm::vec4 &color) = 0;
    virtual void ClearDepth(float depth) = 0;
    virtual void ClearStencil(int stencil) = 0;

    virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
