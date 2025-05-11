#pragma once

#include "Renderer/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
    void SetClearColor(const glm::vec4 &color) override;
    void SetClearDepth(float depth) override;
    void SetClearStencil(int stencil) override;
    void Clear(uint8_t bits) override;

    void EnableDepthTest() override;
    void DisableDepthTest() override;
    void DepthTestFunction(Function fun) override;

    void EnableStencilTest() override;
    void DisableStencilTest() override;
    void StencilFunction(Function fun, int32_t ref, uint32_t mask) override;
    void StencilOperation(Operation sfail, Operation dpfail, Operation dppass) override;

    void EnableBlending() override;
    void DisableBlending() override;
    void BlendFunction(Factor source, Factor destination) override;
    void BlendColor(const glm::vec4 &color) override;

    void EnableCulling() override;
    void DisableCulling() override;
    void CullingFace(Face face) override;
    void FrontFace(Winding winding) override;

    void EnableSeamlessCubemap() override;
    void DisableSeamlessCubemap() override;

    void DrawIndexed(uint32_t count) override;
};

} // namespace sl
