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

void OpenGLRenderAPI::DrawIndexed(uint32_t count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace sl
