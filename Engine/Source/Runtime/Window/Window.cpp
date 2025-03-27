#include "Window.h"

#include "Core/Log.h"
#include "Event/DropEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"

#include <glad/gl.h>
#include <SDL3/SDL.h>

namespace sl
{

void Window::Init()
{

    SL_LOG_INFO("Initializing SDL");

    SL_ASSERT(SDL_Init(SDL_INIT_EVENTS), "Failed to initialize SDL: {}", SDL_GetError());

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void Window::Quit()
{
    SDL_Quit();
}

Window::Window(std::string_view title, uint32_t width, uint32_t height) :
    m_title(title), m_width(width), m_height(height)
{
    SL_LOG_TRACE("Creating window \"{}\"", title);

    uint64_t windowFlags = SDL_WINDOW_RESIZABLE;
    windowFlags |= SDL_WINDOW_OPENGL;

    SDL_Window *pWindow = SDL_CreateWindow(m_title.data(), m_width, m_height, windowFlags);
    if (!pWindow)
    {
        SL_LOG_ERROR("Failed to creat window: {}", SDL_GetError());
        return;
    }
    m_pNativeWindow = pWindow;

    { // TMP
        SL_LOG_TRACE("Creating OpenGL context");

        SDL_GLContext pContext = SDL_GL_CreateContext(pWindow);
        if (!pContext)
        {
            SL_LOG_ERROR("Failed to creat OpenGL context: {}", SDL_GetError());
            return;
        }
        m_pRenderContext = pContext;

        SDL_GL_MakeCurrent(pWindow, pContext);
        SDL_GL_SetSwapInterval(1);

        int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
        SL_LOG_TRACE("\tVersion: {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    }

    SDL_SetWindowPosition(pWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_ShowWindow(pWindow);
}

Window::~Window()
{
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(m_pRenderContext));
    SDL_DestroyWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

void Window::BeginFrame()
{
    // TMP
    glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    PullEvents();
}

void Window::EndFrame()
{
    SDL_GL_MakeCurrent(static_cast<SDL_Window *>(m_pNativeWindow), static_cast<SDL_GLContext>(m_pRenderContext));
    SDL_GL_SwapWindow(static_cast<SDL_Window *>(m_pNativeWindow));
}

void Window::PullEvents()
{
    SDL_Event SDLevent;
    while (SDL_PollEvent(&SDLevent))
    {
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
                if (ImGuiContext::WantCaptureKeyboard())
                {
                    break;
                }

                KeyDownEvent event(SDLevent.key.scancode, SDLevent.key.mod, SDLevent.key.repeat);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_KEY_UP:
            {
                if (ImGuiContext::WantCaptureKeyboard())
                {
                    break;
                }

                KeyUpEvent event(SDLevent.key.scancode);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_TEXT_INPUT:
            {
                if (ImGuiContext::WantCaptureKeyboard())
                {
                    break;
                }

                KeyTypeEvent event(SDLevent.text.text);
                m_eventCallback(event);
                break;
            }

            // Mouse events
            case SDL_EVENT_MOUSE_MOTION:
            {
                if (ImGuiContext::WantCaptureMouse())
                {
                    break;
                }

                MouseMoveEvent event(SDLevent.motion.x, SDLevent.motion.y);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            {
                if (ImGuiContext::WantCaptureMouse())
                {
                    break;
                }

                MouseButtonDownEvent event(SDLevent.button.button, SDLevent.button.clicks == 2);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                if (ImGuiContext::WantCaptureMouse())
                {
                    break;
                }

                MouseButtonUpEvent event(SDLevent.button.button);
                m_eventCallback(event);
                break;
            }
            case SDL_EVENT_MOUSE_WHEEL:
            {
                if (ImGuiContext::WantCaptureMouse())
                {
                    break;
                }

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
