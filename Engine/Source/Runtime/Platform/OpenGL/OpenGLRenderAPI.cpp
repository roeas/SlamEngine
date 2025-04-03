#include "OpenGLRenderAPI.h"

#include "Platform/OpenGL/OpenGLDefines.h"

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

void OpenGLRenderAPI::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderAPI::DrawIndexed(uint32_t count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace sl
