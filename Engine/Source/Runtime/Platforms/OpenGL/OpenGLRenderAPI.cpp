#include "OpenGLRenderAPI.h"

#include "Platforms/OpenGL/OpenGLDefines.h"

#include <glad/gl.h>

namespace sl
{

void OpenGLRenderAPI::SetClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderAPI::SetClearDepth(float depth)
{
    glClearDepth(depth);
}

void OpenGLRenderAPI::SetClearStencil(int stencil)
{
    glClearStencil(stencil);
}

void OpenGLRenderAPI::Clear(uint8_t bits)
{
    glClear(GLClearBufferBit[(bits & SL_CLEAR_COLOR_MASK) >> SL_CLEAR_COLOR_SHIFT] |
        GLClearBufferBit[(bits & SL_CLEAR_DEPTH_MASK) >> SL_CLEAR_DEPTH_SHIFT] |
        GLClearBufferBit[(bits & SL_CLEAR_STENCIL_MASK) >> SL_CLEAR_STENCIL_SHIFT]);
};

void OpenGLRenderAPI::ClearTextureSlot(uint32_t slot)
{
    glBindTextureUnit(slot, 0);
}

void OpenGLRenderAPI::Culling(bool enable)
{
    enable ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void OpenGLRenderAPI::CullingFace(Face face)
{
    glCullFace(GLCullingMode[(size_t)face]);
}

void OpenGLRenderAPI::FrontFace(Winding winding)
{
    glFrontFace(GLWinding[(size_t)winding]);
}

void OpenGLRenderAPI::StencilTest(bool enable)
{
    enable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
}

void OpenGLRenderAPI::StencilFunction(Function fun, int32_t ref, uint32_t mask)
{
    glStencilFunc(GLFunction[(size_t)fun], ref, mask);
}

void OpenGLRenderAPI::StencilOperation(Operation sfail, Operation dpfail, Operation dppass)
{
    glStencilOp(GLOperation[(size_t)sfail], GLOperation[(size_t)dpfail], GLOperation[(size_t)dppass]);
}

void OpenGLRenderAPI::DepthTest(bool enable)
{
    enable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderAPI::WriteDepthBuffer(bool enable)
{
    glDepthMask(enable ? GL_TRUE : GL_FALSE);
}

void OpenGLRenderAPI::DepthTestFunction(Function fun)
{
    glDepthFunc(GLFunction[(size_t)fun]);
}

void OpenGLRenderAPI::Blending(bool enable)
{
    enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void OpenGLRenderAPI::BlendFunction(Factor source, Factor destination)
{
    glBlendFunc(GLFactor[(size_t)source], GLFactor[(size_t)destination]);
}

void OpenGLRenderAPI::BlendColor(const glm::vec4 &color)
{
    glBlendColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderAPI::SeamlessCubemap(bool enable)
{
    enable ? glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS) : glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void OpenGLRenderAPI::DrawIndexed(uint32_t count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace sl
