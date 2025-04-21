#include "ImGuiLayer.h"

#include "Core/Path.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiContext.h"
#include "Window/Input.h"

#include "Panels/AssetBrowser.h"
#include "Panels/Details.h"
#include "Panels/EntityList.h"
#include "Panels/MenuBar.h"
#include "Panels/OutputLog.h"
#include "Panels/State.h"
#include "Panels/ViewPort.h"

#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>
#include <implot/implot.h>

ImGuiLayer::ImGuiLayer() : m_pMainWindow(nullptr)
{
    ImGui::GetIO().UserData = &m_data;

    auto pMenuBar = std::make_unique<MenuBar>();
    auto pState = std::make_unique<State>();
    auto pOutputLog = std::make_unique<OutputLog>();
    auto pAssetBrowser = std::make_unique<AssetBrowser>();
    auto pEntityList = std::make_unique<EntityList>();
    auto pDetails = std::make_unique<Details>();
    auto pViewport = std::make_unique<Viewport>();

    m_panelStack.PushLayer(std::move(pMenuBar));
    m_panelStack.PushLayer(std::move(pState));
    m_panelStack.PushLayer(std::move(pOutputLog));
    m_panelStack.PushLayer(std::move(pAssetBrowser));
    m_panelStack.PushLayer(std::move(pEntityList));
    m_panelStack.PushLayer(std::move(pDetails));
    m_panelStack.PushLayer(std::move(pViewport));
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
    m_panelStack.BeginFrame();
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
    if (m_data.m_debugImGuizmoState)
    {
        // ImGuizmo debug panel
        ImGui::Begin("ImGuizmo State", &m_data.m_debugImGuizmoState, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::TextUnformatted("Is over: ");
        ImGui::SameLine();
        ImGui::TextUnformatted(m_data.m_ImGuizmoIsOver ? "true" : "fasle");
        ImGui::TextUnformatted("Is using: ");
        ImGui::SameLine();
        ImGui::TextUnformatted(m_data.m_ImGuizmoIsUsing ? "true" : "fasle");
        ImGui::TextUnformatted("Is using view manipulate: ");
        ImGui::SameLine();
        ImGui::TextUnformatted(m_data.m_ImGuizmoIsUsingViewManipulate ? "true" : "fasle");
        ImGui::TextUnformatted("Is view manipulate hovered: ");
        ImGui::SameLine();
        ImGui::TextUnformatted(m_data.m_ImGuizmoIsViewManipulateHovered ? "true" : "fasle");
        ImGui::TextUnformatted("Is using any: ");
        ImGui::SameLine();
        ImGui::TextUnformatted(m_data.m_ImGuizmoIsUsingAny ? "true" : "fasle");

        ImGui::End(); // ImGuizmo State
    }

    ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockSpace"), ImGui::GetMainViewport(), m_data.m_dockspaceFlag);
    m_panelStack.OnUpdate(deltaTime);

    if (m_data.m_windowShouldClose)
    {
        sl::WindowCloseEvent event;
        m_eventCallback(event);
    }
}

void ImGuiLayer::OnRender()
{
    m_panelStack.OnRender();
    sl::ImGuiContext::Submit();
}

void ImGuiLayer::EndFrame()
{
    m_panelStack.EndFrame();
}

void ImGuiLayer::OnEvent(sl::Event &event)
{
    sl::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<sl::KeyDownEvent>(SL_BIND_EVENT_CALLBACK(ImGuiLayer::OnKeyDownEvent));
    dispatcher.Dispatch<sl::MouseButtonDownEvent>(SL_BIND_EVENT_CALLBACK(ImGuiLayer::OnMouseButtonDown));
    dispatcher.Dispatch<sl::MouseButtonUpEvent>(SL_BIND_EVENT_CALLBACK(ImGuiLayer::OnMouseButtonUp));

    m_panelStack.OnEvent(event);
}

bool ImGuiLayer::OnKeyDownEvent(sl::KeyDownEvent &event)
{
    if (sl::World::GetMainCameraComponent().IsUsing() || ImGuizmo::IsUsing())
    {
        return false;
    }

    // Change transform gizmo operation mode
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    switch (event.GetKey())
    {
        case SL_KEY_Q:
        {
            pData->m_imguizmoMode = -1;
            break;
        }
        case SL_KEY_W:
        {
            pData->m_imguizmoMode = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }
        case SL_KEY_E:
        {
            pData->m_imguizmoMode = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case SL_KEY_R:
        {
            pData->m_imguizmoMode = ImGuizmo::OPERATION::SCALE;
            break;
        }
        case SL_KEY_T:
        {
            pData->m_imguizmoMode = ImGuizmo::OPERATION::UNIVERSAL;
            break;
        }
        default:
        {
            break;
        }
    }

    return false;
}

bool ImGuiLayer::OnMouseButtonDown(sl::MouseButtonDownEvent &event)
{
    // Right button down to start moving camera
    if (event.GetButton() == SL_MOUSE_BUTTON_RIGHT && m_data.m_isMouseHoverViewport)
    {
        sl::World::GetMainCameraComponent().m_controllerMode = sl::CameraControllerMode::FPS;
        sl::Input::SetMouseRelativeMode(m_pMainWindow, true);
        sl::Input::GetMouseDelta();
        return true;
    }
    else if (event.GetButton() == SL_MOUSE_BUTTON_X1 && m_data.m_isMouseHoverAssetBrowser &&
        m_data.m_assetBrowserCrtPath != sl::Path::GetAsset())
    {
        // TODO: Go forward
        m_data.m_assetBrowserCrtPath = m_data.m_assetBrowserCrtPath.parent_path();
        return true;
    }
    return false;
}

bool ImGuiLayer::OnMouseButtonUp(sl::MouseButtonUpEvent &event)
{
    // Left button up to stop moving camera
    if (event.GetButton() == SL_MOUSE_BUTTON_RIGHT &&
        sl::World::GetMainCameraComponent().m_controllerMode != sl::CameraControllerMode::None)
    {
        sl::World::GetMainCameraComponent().m_controllerMode = sl::CameraControllerMode::None;
        sl::Input::SetMouseRelativeMode(m_pMainWindow, false);
        return true;
    }
    return false;
}

static_assert(ImGuizmo::OPERATION::TRANSLATE == 7);
static_assert(ImGuiDockNodeFlags_None == 0);
