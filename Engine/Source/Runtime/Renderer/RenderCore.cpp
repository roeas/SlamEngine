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

void RenderCore::DepthTest(bool enable)
{
    if (enable)
    {
        pRenderAPI->EnableDepthTest();
    }
    else
    {
        pRenderAPI->DisableDepthTest();
    }
}

void RenderCore::DepthTestFunction(Function fun)
{
    pRenderAPI->EnableDepthTest();
    pRenderAPI->DepthTestFunction(fun);
}

void RenderCore::StencilTest(bool enable)
{
    if (enable)
    {
        pRenderAPI->EnableStencilTest();
    }
    else
    {
        pRenderAPI->DisableStencilTest();
    }
}

void RenderCore::StencilFunction(Function fun, int32_t ref, uint32_t mask)
{
    pRenderAPI->EnableDepthTest();
    pRenderAPI->StencilFunction(fun, ref, mask);
}

void RenderCore::StencilOperation(Operation sfail, Operation dpfail, Operation dppass)
{
    pRenderAPI->EnableDepthTest();
    pRenderAPI->StencilOperation(sfail, dpfail, dppass);
}

void RenderCore::Blending(bool enable)
{
    if (enable)
    {
        pRenderAPI->EnableBlending();
    }
    else
    {
        pRenderAPI->DisableBlending();
    }
}

void RenderCore::BlendFunction(Factor source, Factor destination)
{
    pRenderAPI->EnableBlending();
    pRenderAPI->BlendFunction(source, destination);
}

void RenderCore::BlendColor(const glm::vec4 &color)
{
    pRenderAPI->EnableBlending();
    pRenderAPI->BlendColor(color);
}

void RenderCore::Culling(bool enable)
{
    if (enable)
    {
        pRenderAPI->EnableCulling();
    }
    else
    {
        pRenderAPI->DisableCulling();
    }
}

void RenderCore::CullingFace(Face face)
{
    pRenderAPI->EnableCulling();
    pRenderAPI->CullingFace(face);
}

void RenderCore::FrontFace(Winding winding)
{
    pRenderAPI->EnableCulling();
    pRenderAPI->FrontFace(winding);
}

void RenderCore::SeamlessCubemap(bool enable)
{
    if (enable)
    {
        pRenderAPI->EnableSeamlessCubemap();
    }
    else
    {
        pRenderAPI->DisableSeamlessCubemap();
    }
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
