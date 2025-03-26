#pragma once

#include "Event/Event.h"

namespace sl
{

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

    std::string_view GetTitlke() const { return m_title; }
    std::tuple<uint32_t, uint32_t> GetSize() const { return { m_width, m_height }; }
    void *GetNativeWindow() const { return m_pNativeWindow; }
    void *GetRenderContext() const { return m_pRenderContext; }

    void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
    void PullEvents();

    std::string m_title;
    uint32_t m_width, m_height;
    void *m_pNativeWindow = nullptr;
    void *m_pRenderContext = nullptr;

    EventCallback m_eventCallback;
};

} // namespace sl
