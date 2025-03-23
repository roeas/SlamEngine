#include "Editor.h"

#include "Core/Log.h"
#include "Window/Window.h"

namespace sl
{

Editor::Editor(const EditorInitor &initor)
{
    Log::Init();
    Window::Init();
    m_pWindow = std::make_unique<Window>();
    m_pWindow->Create("", 0, 0);
}

Editor::~Editor()
{

}

void Editor::Run()
{

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
