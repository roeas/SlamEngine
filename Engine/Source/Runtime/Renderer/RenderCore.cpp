#include "RenderCore.h"

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

void RenderCore::DrawIndexed(uint32_t count)
{
    m_pRenderAPI->DrawIndexed(count);
}

} // namespace sl
