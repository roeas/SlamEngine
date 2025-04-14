#pragma once

#include "Core/Defines.h"
#include "Core/Time.h"
#include "Layer/LayerStack.h"

namespace sl
{

class Event;
class Window;
class WindowCloseEvent;
class WindowMinimizeEvent;
class WindowRestoreEvent;

}

struct EditorInitor
{
    std::string_view m_title;
    uint32_t m_width;
    uint32_t m_height;
    sl::GraphicsBackend m_backend;
};

class Editor final
{
public:
    Editor() = delete;
    Editor(const Editor &) = delete;
    Editor &operator=(const Editor &) = delete;
    Editor(Editor &&) = delete;
    Editor &operator=(Editor &&) = delete;

    Editor(const EditorInitor &initor);
    ~Editor();

    void Run();

private:
    void BeginFrame();
    void Update();
    void Render();
    void EndFrame();

    void OnEvent(sl::Event &event);
    bool OnWindowClose(sl::WindowCloseEvent &event);
    bool OnWindowMinimize(sl::WindowMinimizeEvent &event);
    bool OnWindowRestore(sl::WindowRestoreEvent &event);

    sl::Clock m_clock;
    sl::LayerStack m_layerStack;
    std::unique_ptr<sl::Window> m_pMainWindow;

    bool m_isRunning = true;
    bool m_isMinimized = false;
};
