#include "Editor.h"

#include "Core/Log.h"
#include "Window/Window.h"

namespace sl
{

Editor::Editor(const EditorInitor &initor)
{
    Log::Init();
    Window::Init();
    m_pWindow = std::make_unique<Window>("SlamEngine", 1280, 720);
}

Editor::~Editor()
{
    Window::Quit();
}

void Editor::Run()
{
    while (m_isRunning)
    {
        SL_LOG_DEBUG("Running");
    }
}

void Editor::BegineFrame()
{

}

void Editor::Update()
{

}

void Editor::Render()
{

}

void Editor::EndFrame()
{

}

} // namespace sl
