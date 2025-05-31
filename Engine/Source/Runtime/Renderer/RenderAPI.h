#pragma once

#include "Core/Defines.h"

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
    virtual void Clear(uint8_t bits) = 0;

    virtual void DepthTest(bool enable) = 0;
    virtual void WriteDepthBuffer(bool enable) = 0;
    virtual void DepthTestFunction(Function fun) = 0;

    virtual void StencilTest(bool enable) = 0;
    virtual void StencilFunction(Function fun, int32_t ref, uint32_t mask) = 0;
    virtual void StencilOperation(Operation sfail, Operation dpfail, Operation dppass) = 0;

    virtual void Blending(bool enable) = 0;
    virtual void BlendFunction(Factor source, Factor destination) = 0;
    virtual void BlendColor(const glm::vec4 &color) = 0;

    virtual void Culling(bool enable) = 0;
    virtual void CullingFace(Face face) = 0;
    virtual void FrontFace(Winding winding) = 0;

    virtual void SeamlessCubemap(bool enable) = 0;

    virtual void ClearTextureSlot(uint32_t slot) = 0;
    virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
