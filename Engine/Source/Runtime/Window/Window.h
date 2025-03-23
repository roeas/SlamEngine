#pragma once

#include "Event/Event.h"

namespace sl
{

class Window final
{
public:
    static void Init();

public:
    Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;
    ~Window();

    void Create(std::string_view title, uint32_t width, uint32_t height);
    void Terminate();

    void BegineFrame();
    void EndFrame();

    void *GetNativeWindow() const { return m_pNativeWindow; }
    std::tuple<uint32_t, uint32_t> GetSize() const;

    void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
    void PullEvents();

    void *m_pNativeWindow = nullptr;
    EventCallback m_eventCallback;
};

} // namespace sl
