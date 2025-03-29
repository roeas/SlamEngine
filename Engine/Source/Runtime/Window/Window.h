#pragma once

#include "Event/Event.h"

#include <glm/vec2.hpp>

namespace sl
{

class RenderContext;

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
    std::string_view GetTitlke() const { return m_title; }
    glm::u32vec2 GetSize() const { return { m_width, m_height }; }

    void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
    void PullEvents();

    EventCallback m_eventCallback;

    std::string m_title;
    void *m_pNativeWindow;
    std::unique_ptr<RenderContext> m_pContext;
    uint32_t m_width, m_height;
};

} // namespace sl
