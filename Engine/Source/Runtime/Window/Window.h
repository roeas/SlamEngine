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
    Window(std::string_view title, uint32_t width, uint32_t height);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;
    ~Window();

    void BeginFrame();
    void EndFrame();

    void *GetNativeWindow() const;
    void *GetRenderContext() const;
    std::string_view GetTitle() const { return m_title; }
    glm::uvec2 GetSize() const { return { m_width, m_height }; }
    void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
    void PullEvents();

    EventCallback m_eventCallback;
    std::unique_ptr<GraphicsContext> m_pContext;
    
    void *m_pNativeWindow;
    std::string m_title;
    uint32_t m_width, m_height;
};

} // namespace sl
