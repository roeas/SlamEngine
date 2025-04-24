#include "RenderCore.h"

#include "Core/Log.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace sl
{

void RenderCore::Init(GraphicsBackend backend)
{
    m_backend = backend;
    m_pRenderAPI.reset(RenderAPI::Create());
}

void RenderCore::SetMainFramebuffer(std::unique_ptr<FrameBuffer> pFrameBuffer)
{
    m_pMainFrameBuffer = std::move(pFrameBuffer);
}

void RenderCore::SetEntityIDFramebuffer(std::unique_ptr<FrameBuffer> pFrameBuffer)
{
    m_pEntityIDFrameBuffer = std::move(pFrameBuffer);
}

void RenderCore::ClearColor(const glm::vec4 &color)
{
    m_pRenderAPI->SetClearColor(color);
    m_pRenderAPI->Clear(SL_CLEAR_COLOR);
}

void RenderCore::ClearDepth(float depth)
{
    m_pRenderAPI->SetClearDepth(depth);
    m_pRenderAPI->Clear(SL_CLEAR_DEPTH);
}

void RenderCore::ClearStencil(int stencil)
{
    m_pRenderAPI->SetClearStencil(stencil);
    m_pRenderAPI->Clear(SL_CLEAR_STENCIL);
}

void RenderCore::Submit(VertexArray *pVertexArray, Shader *pShader)
{
    pVertexArray->Bind();
    pShader->Bind();

    m_pRenderAPI->DrawIndexed(pVertexArray->GetIndexCount());

    pVertexArray->Unbind();
    pShader->Unbind();
}

} // namespace sl
