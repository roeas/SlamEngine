#include "Editor.h"

#include "Core/Log.h"
#include "Event/WindowEvent.h"
#include "Layer/LayerStack.h"
#include "Window/Window.h"

Editor::Editor(const EditorInitor &initor)
{
    sl::Log::Init();
    sl::Window::Init();
    m_pMainWindow = std::make_unique<sl::Window>("SlamEngine", 1280, 720);
    m_pMainWindow->SetEventCallback(BIND_EVENT_CALLBACK(Editor::OnEvent));

    m_pLayerStack = std::make_unique<sl::LayerStack>();
}

Editor::~Editor()
{
    sl::Window::Quit();
}

void Editor::Run()
{
    while (m_isRunning)
    {
        BeginFrame();

        if (!m_isMinimized)
        {
            Update();
            Render();
        }

        EndFrame();
    }
}

void Editor::BeginFrame()
{
    m_pMainWindow->BeginFrame();
    m_pLayerStack->BeginFrame();
}

void Editor::Update()
{
    m_pLayerStack->Update(0.0f);
}

void Editor::Render()
{
    m_pLayerStack->Render();
}

void Editor::EndFrame()
{
    m_pLayerStack->EndFrame();
    m_pMainWindow->EndFrame();
}

void Editor::OnEvent(sl::Event &event)
{
    sl::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<sl::WindowCloseEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowClose));
    dispatcher.Dispatch<sl::WindowMinimizeEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowMinimize));
    dispatcher.Dispatch<sl::WindowRestoreEvent>(BIND_EVENT_CALLBACK(Editor::OnWindowRestore));

    // Iterate layers from top to bottom / from end to begin
    for (auto it = m_pLayerStack->rend(); it != m_pLayerStack->rbegin(); ++it)
    {
        if (event.IsHandled())
        {
            break;
        }
        (*it)->OnEvent(event);
    }
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
