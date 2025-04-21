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

void RenderCore::SetMainFramebuffer(FrameBuffer *pFrameBuffer)
{
    m_pMainFrameBuffer.reset(pFrameBuffer);
}

void RenderCore::SetEntityIDFramebuffer(FrameBuffer *pFrameBuffer)
{
    m_pEntityIDFrameBuffer.reset(pFrameBuffer);
}

void RenderCore::SetUniformBuffer(std::string_view name, UniformBuffer *pUniformBuffer)
{
    if (m_UniformBuffers.find(name.data()) != m_UniformBuffers.end())
    {
        SL_LOG_ERROR("Uniform buffer {} already exists!", name.data());
        return;
    }

    m_UniformBuffers[name.data()].reset(pUniformBuffer);
}

UniformBuffer *RenderCore::GetUniformBuffer(std::string_view name)
{
    auto it = m_UniformBuffers.find(name.data());
    if (it == m_UniformBuffers.end())
    {
        SL_LOG_ERROR("Uniform buffer {} does not exist!", name.data());
        return nullptr;
    }

    return it->second.get();
}

void RenderCore::ClearColor(const glm::vec4 &color)
{
    m_pRenderAPI->ClearColor(color);
}

void RenderCore::ClearDepth(float depth)
{
    m_pRenderAPI->ClearDepth(depth);
}

void RenderCore::ClearStencil(int stencil)
{
    m_pRenderAPI->ClearStencil(stencil);
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
