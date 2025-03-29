#include "OpenGLContext.h"

#include "Core/Log.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

namespace sl
{

OpenGLContext::OpenGLContext(void *pWindow) : m_pWindow(pWindow)
{
    SL_LOG_INFO("Creating OpenGL context");

    SDL_GLContext pContext = SDL_GL_CreateContext(static_cast<SDL_Window *>(m_pWindow));
    if (!pContext)
    {
        SL_LOG_ERROR("Failed to creat OpenGL context: {}", SDL_GetError());
        return;
    }
    m_pContext = pContext;

    SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pWindow), pContext);
    SDL_GL_SetSwapInterval(1);

    gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);

    SL_LOG_TRACE("Vendor: {}", (const char *)glGetString(GL_VENDOR));
    SL_LOG_TRACE("Renderer: {}", (const char *)glGetString(GL_RENDERER));
    SL_LOG_TRACE("Version: {}", (const char *)glGetString(GL_VERSION));
}

OpenGLContext::~OpenGLContext()
{
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(m_pContext));
}

void OpenGLContext::MakeCurrent()
{
    SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pWindow), static_cast<SDL_GLContext>(m_pContext));
}

void OpenGLContext::SwapBuffers()
{
    SDL_GL_SwapWindow(static_cast<SDL_Window *>(m_pWindow));
}

} // namespace sl
