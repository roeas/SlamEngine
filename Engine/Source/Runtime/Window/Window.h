#pragma once

#include "Event/Event.h"

#include <glm/vec2.hpp>

namespace sl
{

class GraphicsContext;

class Window final
{
public:
    static void Init();
    static void Quit();

public:
    Window(const char *pTitle, uint32_t width, uint32_t height);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;
    ~Window();

    void BeginFrame();
    void EndFrame();

    void *GetNativeWindow() const;
    void *GetRenderContext() const;
    void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
    void PullEvents();

    void *m_pNativeWindow;
    std::unique_ptr<GraphicsContext> m_pContext;
    EventCallback m_eventCallback;
};

} // namespace sl
