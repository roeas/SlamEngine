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

void RenderCore::SetMainFramebuffer(Framebuffer *pFrameBuffer)
{
    pMainFrameBuffer.reset(pFrameBuffer);
}

void RenderCore::SetFinalFramebuffer(Framebuffer *pFrameBuffer)
{
    pFinalFrameBuffer.reset(pFrameBuffer);
}

void RenderCore::SetEntityIDFramebuffer(Framebuffer *pFrameBuffer)
{
    pEntityIDFrameBuffer.reset(pFrameBuffer);
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

void RenderCore::ClearTextureSlot(uint32_t slot)
{
    pRenderAPI->ClearTextureSlot(slot);
}

void RenderCore::Culling(bool enable)
{
    pRenderAPI->Culling(enable);
}

void RenderCore::CullingFace(Face face)
{
    pRenderAPI->Culling(true);
    pRenderAPI->CullingFace(face);
}

void RenderCore::FrontFace(Winding winding)
{
    pRenderAPI->Culling(true);
    pRenderAPI->FrontFace(winding);
}

void RenderCore::StencilTest(bool enable)
{
    pRenderAPI->StencilTest(enable);
}

void RenderCore::StencilFunction(Function fun, int32_t ref, uint32_t mask)
{
    pRenderAPI->DepthTest(true);
    pRenderAPI->StencilFunction(fun, ref, mask);
}

void RenderCore::StencilOperation(Operation sfail, Operation dpfail, Operation dppass)
{
    pRenderAPI->DepthTest(true);
    pRenderAPI->StencilOperation(sfail, dpfail, dppass);
}

void RenderCore::DepthTest(bool enable)
{
    pRenderAPI->DepthTest(enable);
}

void RenderCore::WriteDepthBuffer(bool enable)
{
    pRenderAPI->WriteDepthBuffer(enable);
}

void RenderCore::DepthTestFunction(Function fun)
{
    pRenderAPI->DepthTest(true);
    pRenderAPI->WriteDepthBuffer(true);
    pRenderAPI->DepthTestFunction(fun);
}

void RenderCore::Blending(bool enable)
{
    pRenderAPI->Blending(enable);
}

void RenderCore::BlendFunction(Factor source, Factor destination)
{
    pRenderAPI->Blending(true);
    pRenderAPI->BlendFunction(source, destination);
}

void RenderCore::BlendColor(const glm::vec4 &color)
{
    pRenderAPI->Blending(true);
    pRenderAPI->BlendColor(color);
}

void RenderCore::SeamlessCubemap(bool enable)
{
    pRenderAPI->SeamlessCubemap(enable);
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
