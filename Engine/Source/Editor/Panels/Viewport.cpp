#include "Viewport.h"

#include "Event/MouseEvent.h"
#include "ImGui/IconsMaterialSymbols.h"
#include "Panels/ImGuiData.h"
#include "Panels/ImGuiUtils.h"
#include "Renderer/RenderCore.h"
#include "Scene/World.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace
{

void UpdateImGuizmoDebugState()
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

    pData->m_ImGuizmoIsOver = ImGuizmo::IsOver();
    pData->m_ImGuizmoIsUsing = ImGuizmo::IsUsing();
    pData->m_ImGuizmoIsUsingViewManipulate = ImGuizmo::IsUsingViewManipulate();
    pData->m_ImGuizmoIsViewManipulateHovered = ImGuizmo::IsViewManipulateHovered();
    pData->m_ImGuizmoIsUsingAny = ImGuizmo::IsUsingAny();
}

} // namespace

void Viewport::OnAttach()
{

}

void Viewport::OnDetach()
{

}

void Viewport::BeginFrame()
{

}

void Viewport::OnUpdate(float deltaTime)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    if (!ImGui::Begin("Viewport"))
    {
        ImGui::End();
        return;
    }
    ImGui::PopStyleVar();

    // Is mouse in viewport
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    pData->m_isMouseHoverViewport = ImGui::IsWindowHovered();

    // Window position
    const auto &windowPos = ImGui::GetWindowPos();
    m_windowPosX = (uint32_t)windowPos.x;
    m_windowPosY = (uint32_t)windowPos.y;

    // Window size
    const ImVec2 &crtSize = ImGui::GetContentRegionAvail();
    if (m_windowSizeX != crtSize.x || m_windowSizeY != crtSize.y)
    {
        m_windowSizeX = (uint32_t)crtSize.x;
        m_windowSizeY = (uint32_t)crtSize.y;

        // Resize main framebuffer
        sl::RenderCore::GetMainFramebuffer()->Resize(m_windowSizeX, m_windowSizeY);
        sl::RenderCore::GetEntityIDFramebuffer()->Resize(m_windowSizeX, m_windowSizeY);

        // Update main camera aspect
        float aspect = (float)m_windowSizeX / (float)m_windowSizeY;
        auto &camera = sl::World::GetMainCameraComponent();
        camera.m_aspect = aspect;
        camera.m_fovMultiplier = aspect * 9.0f / 16.0f;
        camera.m_isDirty = true;
    }

    // Draw main frame buffer color attachment
    uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
    ImGui::Image((ImTextureID)handle, ImVec2{ (float)m_windowSizeX, (float)m_windowSizeY },
        ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

    ShowImGuizmoTransform();
    ShowImGuizmoOrientation();
    UpdateImGuizmoDebugState();

    ImGui::End(); // Viewport
    ShowToolOverlay();
}

void Viewport::OnRender()
{

}

void Viewport::EndFrame()
{

}

void Viewport::OnEvent(sl::Event &event)
{
    sl::EventDispatcher dispatcher{ event };
    dispatcher.Dispatch<sl::MouseButtonDownEvent>(SL_BIND_EVENT_CALLBACK(Viewport::OnMouseButtonDown));
}

bool Viewport::OnMouseButtonDown(sl::MouseButtonDownEvent &event)
{
    if (event.GetButton() == SL_MOUSE_BUTTON_LEFT)
    {
        return MousePick();
    }

    return true;
}

bool Viewport::MousePick()
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    if (!pData->m_isMouseHoverViewport || pData->m_ImGuizmoIsUsingAny || pData->m_ImGuizmoIsOver ||
        pData->m_ImGuizmoIsViewManipulateHovered || sl::World::GetMainCameraComponent().IsUsing())
    {
        return false;
    }

    // Origin is on the upper left of viewport
    uint32_t mouseLocalPosX = (uint32_t)ImGui::GetMousePos().x - m_windowPosX;
    uint32_t mouseLocalPosY = (uint32_t)ImGui::GetMousePos().y - (m_windowPosY + (uint32_t)GetTitleBarSize());
    if (mouseLocalPosX >= m_windowSizeX || mouseLocalPosY >= m_windowSizeY)
    {
        return false;
    }

    // We clear EntityID buffer by -1 every frame in #RendererLayer
    int entityID = sl::RenderCore::GetEntityIDFramebuffer()->ReadPixel(0, mouseLocalPosX, mouseLocalPosY);
    if (entityID < 0)
    {
        pData->m_selectedEntity.ResetHandle();
        return false;
    }

    sl::Entity crtEntity{ (uint32_t)entityID };
    if (crtEntity == pData->m_selectedEntity)
    {
        return false;
    }

    SL_LOG_TRACE("Select entity, ID: {}, Name: {}, Mouse position: ({}, {})",
        entityID, crtEntity.GetComponents<sl::TagComponent>().m_name.c_str(),
        mouseLocalPosX, mouseLocalPosY);

    pData->m_selectedEntity = crtEntity;
    return true;
}

void Viewport::ShowImGuizmoTransform()
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    if (pData->m_imguizmoMode < 0 || !pData->m_selectedEntity.IsValid() || pData->m_selectedEntity == sl::World::GetMainCameraEntity())
    {
        return;
    }

    // Disable ImGuizmo when camera moving
    auto &camera = sl::World::GetMainCameraComponent();
    ImGuizmo::Enable(!camera.IsUsing());

    ImGuizmo::AllowAxisFlip(false);
    ImGuizmo::SetOrthographic(camera.m_projectionType == sl::ProjectionType::Orthographic);
    ImGuizmo::SetRect((float)m_windowPosX, (float)m_windowPosY + GetTitleBarSize(), (float)m_windowSizeX, (float)m_windowSizeY);

    const glm::mat4 &viewMat = camera.GetView();
    const glm::mat4 &projectionMat = camera.GetProjection();
    auto &transform = pData->m_selectedEntity.GetComponents<sl::TransformComponent>();
    glm::mat4 manipulatedTransformMat = transform.GetTransform();

    ImGuizmo::SetDrawlist();
    ImGuizmo::Manipulate(glm::value_ptr(viewMat), glm::value_ptr(projectionMat),
        (ImGuizmo::OPERATION)pData->m_imguizmoMode, ImGuizmo::LOCAL,
        glm::value_ptr(manipulatedTransformMat), nullptr, nullptr);

    if (ImGuizmo::IsUsing())
    {
        // Decompose transform matrix to position, rotation and scale
        glm::vec3 newPosition, newRotation, newScale;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(manipulatedTransformMat),
            glm::value_ptr(newPosition), glm::value_ptr(newRotation), glm::value_ptr(newScale));

        transform.m_position = newPosition;
        transform.m_rotation = glm::radians(newRotation);
        transform.m_scale = newScale;
    }
}

void Viewport::ShowImGuizmoOrientation()
{
    constexpr float Length = 128.0f;
    ImVec2 pos = ImVec2{
        (float)m_windowPosX + (float)m_windowSizeX - Length,
        (float)m_windowPosY + GetTitleBarSize() };

    // Display a cube on the upper right corner
    auto view = sl::World::GetMainCameraComponent().GetView();
    ImGuizmo::ViewManipulate(glm::value_ptr(view), Length, pos, ImVec2{ Length , Length }, 0);
}

void Viewport::ShowToolOverlay()
{
    constexpr size_t ToolCount = 5;
    constexpr std::array<int, ToolCount> Operations =
    {
        -1, // No ImGuizmo tramsform
        ImGuizmo::OPERATION::TRANSLATE,
        ImGuizmo::OPERATION::ROTATE,
        ImGuizmo::OPERATION::SCALE,
        ImGuizmo::OPERATION::UNIVERSAL,
    };
    constexpr std::array<const char *, ToolCount> Icons =
    {
        ICON_MS_ARROW_SELECTOR_TOOL, ICON_MS_DRAG_PAN, ICON_MS_CACHED, ICON_MS_ZOOM_OUT_MAP, ICON_MS_CROP_ROTATE,
    };

    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    auto ToolButton = [pData](size_t index)
    {
        const int op = Operations[index];
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, pData->m_imguizmoMode == op ? 1.0f : 0.5f);
        if (ImGui::Button(Icons[index], ImVec2{ 40.0f, 40.0f }))
        {
            pData->m_imguizmoMode = op;
        }
        ImGui::PopStyleVar();
    };

    // Display transform tools on the upper left corner
    ImGui::SetNextWindowPos(ImVec2{ (float)m_windowPosX + 10.0f, (float)m_windowPosY + 10.0f + GetTitleBarSize() });
    ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
    for (size_t i = 0; i < ToolCount; ++i)
    {
        ToolButton(i);
    }

    ImGui::End(); // Tools
}
