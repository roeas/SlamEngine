#include "ImGuiLayer.h"

#include "Core/Log.h"
#include "ImGui/ImGuiContext.h"

#include <ImGui/IconsMaterialSymbols.h>
#include <imgui/imgui.h>

ImGuiLayer::ImGuiLayer()
{

}

ImGuiLayer::~ImGuiLayer()
{

}

void ImGuiLayer::OnAttach()
{

}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::OnEvent(sl::Event &event)
{

}

void ImGuiLayer::BeginFrame()
{
    sl::ImGuiContext::NewFrame();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
    ImGui::ShowDemoWindow();
}

void ImGuiLayer::OnRender()
{
    sl::ImGuiContext::Submit();
}

void ImGuiLayer::EndFrame()
{

}
