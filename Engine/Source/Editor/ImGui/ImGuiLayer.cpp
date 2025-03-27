#include "ImGuiLayer.h"

#include "ImGui/ImGuiContext.h"
#include "ImGui/MenuBar.h"
#include "ImGui/WindowInformation.h"

#include <imgui/imgui.h>
#include <implot/implot.h>

ImGuiLayer::ImGuiLayer()
{
    ImGui::GetIO().UserData = &m_data;

    auto pMenuBar = std::make_unique<MenuBar>();
    pMenuBar->SetEventCallback(BIND_EVENT_CALLBACK(ImGuiLayer::ForwardEvent));
    auto pWindowInformation = std::make_unique<WindowInformation>();

    m_stack.PushLayer(std::move(pMenuBar));
    m_stack.PushLayer(std::move(pWindowInformation));
}

void ImGuiLayer::OnAttach()
{

}

void ImGuiLayer::OnDetach()
{

}

void ImGuiLayer::BeginFrame()
{
    sl::ImGuiContext::NewFrame();

    m_stack.BeginFrame();
}

void ImGuiLayer::OnUpdate(float deltaTime)
{
    if (m_data.m_debugImGuiDemo)
    {
        ImGui::ShowDemoWindow(&m_data.m_debugImGuiDemo);
    }
    if (m_data.m_debugImPlotDemo)
    {
        ImPlot::ShowDemoWindow(&m_data.m_debugImPlotDemo);
    }

    ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockSpace"), ImGui::GetMainViewport(), m_data.m_dockspaceFlag);

    m_stack.OnUpdate(deltaTime);
}

void ImGuiLayer::OnRender()
{
    sl::ImGuiContext::Submit();

    m_stack.OnRender();
}

void ImGuiLayer::EndFrame()
{
    m_stack.EndFrame();
}

void ImGuiLayer::OnEvent(sl::Event &event)
{
    m_stack.OnEvent(event);
}

void ImGuiLayer::ForwardEvent(sl::Event &event)
{
    m_eventCallback(event);
}
