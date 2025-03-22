#include "Window.h"

#include "Core/Log.h"

#include <SDL3/SDL.h>

namespace sl
{

void Window::Init()
{
    SL_ASSERT(SDL_Init(SDL_INIT_EVENTS), "Failed to initialize SDL: {}", SDL_GetError());
    SL_LOG_INFO("SDL initialized.");
}

Window::Window()
{

}

Window::~Window()
{

}

void Window::Create(std::string_view title, uint32_t width, uint32_t height)
{

}

void Window::Terminate()
{

}

void Window::BegineFrame()
{

}

void Window::EndFrame()
{

}

std::tuple<uint32_t, uint32_t> Window::GetSize() const
{
    return { 0,0 };
}

void Window::PullEvents()
{

}

} // namespace sl
