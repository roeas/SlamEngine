#include "Editor.h"

#include "Core/Log.h"
#include "Event/WindowEvent.h"
#include "Window/Window.h"

Editor::Editor(const EditorInitor &initor)
{
    sl::Log::Init();
    sl::Window::Init();
    m_pWindow = std::make_unique<sl::Window>("SlamEngine", 1280, 720);
    m_pWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));
}

Editor::~Editor()
{
    sl::Window::Quit();
}

void Editor::Run()
{
    while (m_isRunning)
    {
        BegineFrame();

        if (!m_isMinimized)
        {
            Update();
            Render();
        }

        EndFrame();
    }
}

void Editor::BegineFrame()
{
    m_pWindow->BegineFrame();
}

void Editor::Update()
{

}

void Editor::Render()
{

}

void Editor::EndFrame()
{
    m_pWindow->EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
    sl::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
    dispatcher.Dispatch<sl::WindowMinimizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowMinimize));
    dispatcher.Dispatch<sl::WindowRestoreEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowRestore));
}

bool Editor::OnWindowClose(sl::WindowCloseEvent &event)
{
    SL_LOG_TRACE("Window close");
    m_isRunning = false;
    return true;
}

bool Editor::OnWindowMinimize(sl::WindowMinimizeEvent &event)
{
    SL_LOG_TRACE("Window minimise");
    m_isMinimized = true;
    return true;
}

bool Editor::OnWindowRestore(sl::WindowRestoreEvent &event)
{
    SL_LOG_TRACE("Window restore");
    m_isMinimized = false;
    return true;
}
