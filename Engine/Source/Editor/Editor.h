#pragma once

#include <cstdint>
#include <memory>

namespace sl
{

class Window;

struct EditorInitor
{
    const char *m_title;
    uint32_t m_width;
    uint32_t m_height;
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
    void BegineFrame();
    void Update();
    void Render();
    void EndFrame();

    bool m_isRunning = true;
    bool m_isMinimized = false;

    std::unique_ptr<Window> m_pWindow;
};

} // namespace sl
