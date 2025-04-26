#include "OpenGLContext.h"

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"
#include "Utils/ProfilerGPU.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

namespace sl
{

OpenGLContext::OpenGLContext(void *pWindow) : m_pWindow(pWindow)
{
    SL_PROFILE;
    SL_LOG_INFO("Creating OpenGL context");

    SDL_GLContext pContext = SDL_GL_CreateContext(static_cast<SDL_Window *>(m_pWindow));
    if (!pContext)
    {
        SL_LOG_ERROR("Failed to creat OpenGL context: {}", SDL_GetError());
        return;
    }
    m_pContext = pContext;

    SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pWindow), static_cast<SDL_GLContext>(m_pContext));
    SDL_GL_SetSwapInterval(1);

    gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
    SL_LOG_TRACE("\tVendor: {}", (char *)glGetString(GL_VENDOR));
    SL_LOG_TRACE("\tRenderer: {}", (char *)glGetString(GL_RENDERER));
    SL_LOG_TRACE("\tVersion: {}", (char *)glGetString(GL_VERSION));

    SL_PROFILE_GPU_CONTEXT;
}

OpenGLContext::~OpenGLContext()
{
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(m_pContext));
}

void OpenGLContext::MakeCurrent()
{
    SL_PROFILE;

    SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pWindow), static_cast<SDL_GLContext>(m_pContext));
}

void OpenGLContext::SwapBuffers()
{
    SL_PROFILE_GPU("Swap Buffers");
    SL_PROFILE;

    SDL_GL_SwapWindow(static_cast<SDL_Window *>(m_pWindow));
    
    SL_PROFILE_GPU_COLLECT;
}

} // namespace sl
