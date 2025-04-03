#pragma once

#include "Renderer/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
    ~OpenGLRenderAPI() = default;

    void SetClearColor(const glm::vec4 &color) override;
    void SetClearDepth(float depth) override;
    void SetClearStencil(int stencil) override;
    void Clear() override;

    void DrawIndexed(uint32_t count) override;
};

} // namespace sl
