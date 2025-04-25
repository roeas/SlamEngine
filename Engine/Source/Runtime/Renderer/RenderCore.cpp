#include "RenderCore.h"

#include "Core/Log.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace sl
{

void RenderCore::Init(GraphicsBackend backend)
{
    m_backend = backend;
    pRenderAPI.reset(RenderAPI::Create());
}

void RenderCore::SetMainFramebuffer(std::unique_ptr<FrameBuffer> pFrameBuffer)
{
    pMainFrameBuffer = std::move(pFrameBuffer);
}

void RenderCore::SetEntityIDFramebuffer(std::unique_ptr<FrameBuffer> pFrameBuffer)
{
    pEntityIDFrameBuffer = std::move(pFrameBuffer);
}

void RenderCore::ClearColor(const glm::vec4 &color)
{
    pRenderAPI->SetClearColor(color);
    pRenderAPI->Clear(SL_CLEAR_COLOR);
}

void RenderCore::ClearDepth(float depth)
{
    pRenderAPI->SetClearDepth(depth);
    pRenderAPI->Clear(SL_CLEAR_DEPTH);
}

void RenderCore::ClearStencil(int stencil)
{
    pRenderAPI->SetClearStencil(stencil);
    pRenderAPI->Clear(SL_CLEAR_STENCIL);
}

void RenderCore::Submit(VertexArray *pVertexArray, Shader *pShader)
{
    pVertexArray->Bind();
    pShader->Bind();

    pRenderAPI->DrawIndexed(pVertexArray->GetIndexCount());

    pVertexArray->Unbind();
    pShader->Unbind();
}

} // namespace sl
