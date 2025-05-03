#include "Window.h"

#include "Core/Log.h"
#include "Event/DropEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/RenderCore.h"
#include "Utils/ProfilerCPU.h"

#include <SDL3/SDL.h>

namespace sl
{

void Window::Init()
{
    SL_PROFILE;
    SL_LOG_INFO("Initializing SDL");

    bool success = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    SL_ASSERT(success, "Failed to initialize SDL: {}", SDL_GetError());
}

void Window::Quit()
{
    SL_PROFILE;

    SDL_Quit();
}

Window::Window(const char *pTitle, uint32_t width, uint32_t height) : m_pNativeWindow(nullptr)
{
    SL_PROFILE;
    SL_LOG_INFO("Creating window {}", pTitle);

    uint64_t windowFlags = SDL_WINDOW_RESIZABLE;
    switch (RenderCore::GetBackend())
    {
        case GraphicsBackend::OpenGL:
        {
            windowFlags |= SDL_WINDOW_OPENGL;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
            break;
        }
        default:
        {
            SL_ASSERT(false, "Only support OpenGL backend for now!");
            break;
        }
    }

    SDL_Window *pWindow = SDL_CreateWindow(pTitle, width, height, windowFlags);
    if (!pWindow)
    {
        SL_LOG_ERROR("Failed to creat window: {}", SDL_GetError());
        return;
    }
    m_pNativeWindow = pWindow;

    m_pContext.reset(GraphicsContext::Create(static_cast<SDL_Window *>(m_pNativeWindow)));
    SDL_SetWindowPosition(static_cast<SDL_Window *>(m_pNativeWindow), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(static_cast<SDL_Window *>(m_pNativeWindow));

    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_CENTER, "0");
    SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SYSTEM_SCALE, "1");
}

Window::~Window()
{
    SL_PROFILE;

    SDL_DestroyWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

void Window::BeginFrame()
{
    SL_PROFILE;

    PullEvents();
}

void Window::EndFrame()
{
    SL_PROFILE;

    m_pContext->MakeCurrent();
    m_pContext->SwapBuffers();
}

void *Window::GetNativeWindow() const
{
    return m_pNativeWindow;
}

void *Window::GetRenderContext() const
{
    return m_pContext->GetContext();
}

void Window::PullEvents()
{
    SL_PROFILE;

    SDL_Event SDLevent;
    while (SDL_PollEvent(&SDLevent))
    {
        SL_PROFILE_NAME("SDL Event");
        SL_PROFILE_ADD_VALUE(SDLevent.type);

        sl::ImGuiContext::OnEvent(&SDLevent);
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
