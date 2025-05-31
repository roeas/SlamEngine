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
    void ClearTextureSlot(uint32_t slot) override;

    void Culling(bool enable) override;
    void CullingFace(Face face) override;
    void FrontFace(Winding winding) override;

    void StencilTest(bool enable) override;
    void StencilFunction(Function fun, int32_t ref, uint32_t mask) override;
    void StencilOperation(Operation sfail, Operation dpfail, Operation dppass) override;

    void DepthTest(bool enable) override;
    void WriteDepthBuffer(bool enable) override;
    void DepthTestFunction(Function fun) override;

    void Blending(bool enable) override;
    void BlendFunction(Factor source, Factor destination) override;
    void BlendColor(const glm::vec4 &color) override;

    void SeamlessCubemap(bool enable) override;

    void DrawIndexed(uint32_t count) override;
};

} // namespace sl
