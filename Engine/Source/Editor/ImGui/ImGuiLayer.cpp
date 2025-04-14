#include "ImGuiLayer.h"

#include "ImGui/ImGuiContext.h"
#include "ImGui/MenuBar.h"
#include "ImGui/PanelAssetBrowser.h"
#include "ImGui/PanelDetails.h"
#include "ImGui/PanelEntityList.h"
#include "ImGui/PanelInformation.h"
#include "ImGui/PanelLog.h"
#include "ImGui/PanelScene.h"

#include <imgui/imgui.h>
#include <implot/implot.h>

ImGuiLayer::ImGuiLayer()
{
    ImGui::GetIO().UserData = &m_data;

    auto pMenuBar = std::make_unique<MenuBar>();
    auto pPanelInformation = std::make_unique<PanelInformation>();
    auto pPanelLog = std::make_unique<PanelLog>();
    auto pPanelAssetBrowser = std::make_unique<PanelAssetBrowser>();
    auto pPanelEntityList = std::make_unique<PanelEntityList>();
    auto pPanelDetails = std::make_unique<PanelDetails>();
    auto pPanelScene = std::make_unique<PanelScene>();
    pMenuBar->SetEventCallback(SL_BIND_EVENT_CALLBACK(ImGuiLayer::ForwardEvent));

    m_stack.PushLayer(std::move(pMenuBar));
    m_stack.PushLayer(std::move(pPanelInformation));
    m_stack.PushLayer(std::move(pPanelLog));
    m_stack.PushLayer(std::move(pPanelAssetBrowser));
    m_stack.PushLayer(std::move(pPanelEntityList));
    m_stack.PushLayer(std::move(pPanelDetails));
    m_stack.PushLayer(std::move(pPanelScene));
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
    m_stack.OnRender();
    sl::ImGuiContext::Submit();
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
