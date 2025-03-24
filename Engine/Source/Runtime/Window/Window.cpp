#include "Window.h"

#include "Core/Log.h"

#include <SDL3/SDL.h>

namespace sl
{

void Window::Init()
{
    SL_ASSERT(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS), "Failed to initialize SDL: {}", SDL_GetError());

    const char *glVersion = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SL_LOG_INFO("SDL initialized.");
}

void Window::Quit()
{
    SDL_Quit();
}

Window::Window(std::string_view title, uint32_t width, uint32_t height) :
    m_title(title), m_width(width), m_height(height)
{
    uint64_t windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
    windowFlags |= SDL_WINDOW_OPENGL;

    SDL_Window *pWindow = SDL_CreateWindow(m_title.data(), m_width, m_height, windowFlags);
    if (!pWindow)
    {
        SL_LOG_ERROR("Failed to creat window \"{}\": {}", m_title.data(), SDL_GetError());
        return;
    }
    m_pNativeWindow = pWindow;

    SDL_SetWindowPosition(static_cast<SDL_Window *>(m_pNativeWindow), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_GL_SetSwapInterval(1);
    SDL_ShowWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

Window::~Window()
{
    SDL_DestroyWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

void Window::BegineFrame()
{

}

void Window::EndFrame()
{

}

void Window::PullEvents()
{

}

} // namespace sl
