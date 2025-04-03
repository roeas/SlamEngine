#include "RenderCore.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace sl
{

void RenderCore::Init(GraphicsBackend backend)
{
    m_backend = backend;
    m_pRenderAPI.reset(RenderAPI::Create());
}

void RenderCore::SetClearColor(const glm::vec4 &color)
{
    m_pRenderAPI->SetClearColor(color);
}

void RenderCore::SetClearDepth(float depth)
{
    m_pRenderAPI->SetClearDepth(depth);
}

void RenderCore::SetClearStencil(int stencil)
{
    m_pRenderAPI->SetClearStencil(stencil);
}

void RenderCore::Clear()
{
    m_pRenderAPI->Clear();
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
