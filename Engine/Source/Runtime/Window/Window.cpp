#include "Window.h"

#include "Core/Log.h"
#include "Event/DropEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

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

    // TODO: Create render context

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

    SDL_ShowWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

Window::~Window()
{
    SDL_DestroyWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

void Window::BeginFrame()
{
    PullEvents();
}

void Window::EndFrame()
{

}

void Window::PullEvents()
{
    SDL_Event SDLevent;
    while (SDL_PollEvent(&SDLevent))
    {
        switch (SDLevent.type)
        {
            // Window events
            case SDL_EVENT_WINDOW_RESIZED:
            {
                WindowResizeEvent event(SDLevent.window.data1, SDLevent.window.data2);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_WINDOW_MINIMIZED:
            {
                WindowMinimizeEvent event;
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_WINDOW_MAXIMIZED:
            {
                WindowMaximizeEvent event;
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_WINDOW_RESTORED:
            {
                WindowRestoreEvent event;
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
            {
                WindowGetFocusEvent event;
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_WINDOW_FOCUS_LOST:
            {
                WindowLossFocusEvent event;
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            {
                WindowCloseEvent event;
                m_eventCallback(event);
                break;
            }

            // Keyboard events
            case SDL_EVENT_KEY_DOWN:
            {
                KeyDownEvent event(SDLevent.key.scancode, SDLevent.key.mod, SDLevent.key.repeat);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                KeyUpEvent event(SDLevent.key.scancode);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_TEXT_INPUT:
            {
                KeyTypeEvent event(SDLevent.text.text);
                m_eventCallback(event);
                break;
            }

            // Mouse events
            case SDL_EVENT_MOUSE_MOTION:
            {
                MouseMoveEvent event(SDLevent.motion.x, SDLevent.motion.y);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                MouseButtonDownEvent event(SDLevent.button.button, SDLevent.button.clicks == 2);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                MouseButtonUpEvent event(SDLevent.button.button);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL:
            {
                MouseScrollEvent event(SDLevent.wheel.x, SDLevent.wheel.y);
                m_eventCallback(event);
                break;
            }

            // Drop events
            case SDL_EVENT_DROP_FILE:
            {
                DropEvent event(SDLevent.drop.data);
                m_eventCallback(event);
                break;
            }

            default:
            {
                break;
            }
        }
    }
}

} // namespace sl
