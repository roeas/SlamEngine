#pragma once

#include "Core/Defines.h"
#include "Renderer/FrameBuffer.h"
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

    static void SetMainFramebuffer(FrameBuffer *pFrameBuffer);
    static FrameBuffer *GetMainFramebuffer() { return m_pMainFrameBuffer.get(); }
    static void SetEntityIDFramebuffer(FrameBuffer *pFrameBuffer);
    static FrameBuffer *GetEntityIDFramebuffer() { return m_pEntityIDFrameBuffer.get(); }
    static void SetUniformBuffer(std::string_view name, UniformBuffer *pUniformBuffer);
    static UniformBuffer *GetUniformBuffer(std::string_view name);

    static void ClearColor(const glm::vec4 &color);
    static void ClearDepth(float depth);
    static void ClearStencil(int stencil);

    static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
    inline static GraphicsBackend m_backend = GraphicsBackend::None;
    inline static std::unique_ptr<RenderAPI> m_pRenderAPI;
    inline static std::unique_ptr<FrameBuffer> m_pMainFrameBuffer;
    inline static std::unique_ptr<FrameBuffer> m_pEntityIDFrameBuffer;
    inline static std::map<std::string, std::unique_ptr<UniformBuffer>> m_UniformBuffers;
};

} // namespace sl
