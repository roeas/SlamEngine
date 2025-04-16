#include "ImGuiLayer.h"

#include "Event/MouseEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Panel/AssetBrowser.h"
#include "Panel/Details.h"
#include "Panel/EntityList.h"
#include "Panel/MenuBar.h"
#include "Panel/OutputLog.h"
#include "Panel/State.h"
#include "Panel/ViewPort.h"

#include <imgui/imgui.h>
#include <implot/implot.h>

ImGuiLayer::ImGuiLayer()
{
    ImGui::GetIO().UserData = &m_data;

    auto pMenuBar = std::make_unique<MenuBar>();
    auto pState = std::make_unique<State>();
    auto pOutputLog = std::make_unique<OutputLog>();
    auto pAssetBrowser = std::make_unique<AssetBrowser>();
    auto pEntityList = std::make_unique<EntityList>();
    auto pDetails = std::make_unique<Details>();
    auto pViewport = std::make_unique<Viewport>();
    pMenuBar->SetEventCallback(SL_BIND_EVENT_CALLBACK(ImGuiLayer::ForwardEvent));

    m_stack.PushLayer(std::move(pMenuBar));
    m_stack.PushLayer(std::move(pState));
    m_stack.PushLayer(std::move(pOutputLog));
    m_stack.PushLayer(std::move(pAssetBrowser));
    m_stack.PushLayer(std::move(pEntityList));
    m_stack.PushLayer(std::move(pDetails));
    m_stack.PushLayer(std::move(pViewport));
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
