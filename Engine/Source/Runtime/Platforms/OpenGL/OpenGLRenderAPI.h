#pragma once

#include "Renderer/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
    void ClearColor(const glm::vec4 &color) override;
    void ClearDepth(float depth) override;
    void ClearStencil(int stencil) override;

    void DrawIndexed(uint32_t count) override;
};

} // namespace sl
