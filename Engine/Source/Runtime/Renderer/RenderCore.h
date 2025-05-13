#pragma once

#include "Core/Defines.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/RenderAPI.h"
#include "Renderer/UniformBuffer.h"

#include <map>
#include <memory>

namespace sl
{

class Shader;
class VertexArray;

class RenderCore final
{
public:
    RenderCore() = delete;
    RenderCore(const RenderCore &) = delete;
    RenderCore &operator=(const RenderCore &) = delete;
    RenderCore(RenderCore &&) = delete;
    RenderCore &operator=(RenderCore &&) = delete;
    ~RenderCore() = delete;

    static void Init(GraphicsBackend backend);
    static GraphicsBackend GetBackend() { return m_backend; }

    static void SetMainFramebuffer(Framebuffer *pFrameBuffer);
    static Framebuffer *GetMainFramebuffer() { return pMainFrameBuffer.get(); }
    static void SetFinalFramebuffer(Framebuffer *pFrameBuffer);
    static Framebuffer *GetFinalFramebuffer() { return pFinalFrameBuffer.get(); }
    static void SetEntityIDFramebuffer(Framebuffer *pFrameBuffer);
    static Framebuffer *GetEntityIDFramebuffer() { return pEntityIDFrameBuffer.get(); }

    static void ClearColor(const glm::vec4 &color);
    static void ClearDepth(float depth);
    static void ClearStencil(int stencil);

    static void DepthTest(bool enable);
    static void DepthTestFunction(Function fun);

    static void StencilTest(bool enable);
    static void StencilFunction(Function fun, int32_t ref, uint32_t mask);
    static void StencilOperation(Operation sfail, Operation dpfail, Operation dppass);

    static void Blending(bool enable);
    static void BlendFunction(Factor source, Factor destination);
    static void BlendColor(const glm::vec4 &color);

    static void Culling(bool enable);
    static void CullingFace(Face face);
    static void FrontFace(Winding winding);

    static void SeamlessCubemap(bool enable);

    static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
    inline static GraphicsBackend m_backend = GraphicsBackend::None;
    inline static std::unique_ptr<RenderAPI> pRenderAPI;

    // TODO: Can we move these to #RendererLayer?
    inline static std::unique_ptr<Framebuffer> pMainFrameBuffer;
    inline static std::unique_ptr<Framebuffer> pFinalFrameBuffer;
    inline static std::unique_ptr<Framebuffer> pEntityIDFrameBuffer;
};

} // namespace sl
