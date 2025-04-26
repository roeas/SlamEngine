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

    static void SetMainFramebuffer(std::unique_ptr<Framebuffer> pFrameBuffer);
    static Framebuffer *GetMainFramebuffer() { return pMainFrameBuffer.get(); }
    static void SetEntityIDFramebuffer(std::unique_ptr<Framebuffer> pFrameBuffer);
    static Framebuffer *GetEntityIDFramebuffer() { return pEntityIDFrameBuffer.get(); }

    static void ClearColor(const glm::vec4 &color);
    static void ClearDepth(float depth);
    static void ClearStencil(int stencil);

    static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
    inline static GraphicsBackend m_backend = GraphicsBackend::None;
    inline static std::unique_ptr<RenderAPI> pRenderAPI;

    // TODO: Can we move these to #RendererLayer?
    inline static std::unique_ptr<Framebuffer> pMainFrameBuffer;
    inline static std::unique_ptr<Framebuffer> pEntityIDFrameBuffer;
};

} // namespace sl
