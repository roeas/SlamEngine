#include "OpenGLRenderAPI.h"

#include "Platforms/OpenGL/OpenGLDefines.h"

#include <glad/gl.h>

namespace sl
{

void OpenGLRenderAPI::ClearColor(const glm::vec4 &color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderAPI::ClearDepth(float depth)
{
    glClearDepth(depth);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::ClearStencil(int stencil)
{
    glClearStencil(stencil);
    glClear(GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderAPI::DrawIndexed(uint32_t count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace sl
