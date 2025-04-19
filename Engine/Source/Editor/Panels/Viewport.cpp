#include "Viewport.h"

#include "Panels/ImGuiData.h"
#include "Renderer/RenderCore.h"
#include "Scene/World.h"
#include "ImGui/IconsMaterialSymbols.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imguizmo/ImGuizmo.h>

namespace
{

float GetTitleBarSize()
{
    return ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
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
    static_cast<ImGuiData *>(ImGui::GetIO().UserData)->m_isMouseInViewport = ImGui::IsWindowHovered();

    // Window position
    auto windowPos = ImGui::GetWindowPos();
    m_windowPosX = (uint32_t)windowPos.x;
    m_windowPosY = (uint32_t)windowPos.y;

    // Window size
    ImVec2 crtSize = ImGui::GetContentRegionAvail();
    if (m_windowSizeX != crtSize.x || m_windowSizeY != crtSize.y)
    {
        m_windowSizeX = (uint32_t)crtSize.x;
        m_windowSizeY = (uint32_t)crtSize.y;

        // Resize main framebuffer
        sl::RenderCore::GetMainFramebuffer()->Resize(m_windowSizeX, m_windowSizeY);

        // Update main camera aspect
        float aspect = (float)m_windowSizeX / (float)m_windowSizeY;
        auto &camera = sl::World::GetMainCameraComponent();
        camera.m_aspect = aspect;
        camera.m_fovMultiplier = aspect * 9.0f / 16.0f;
        camera.m_isDirty = true;
    }

    // Draw main frame buffer color attachment
    uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
    ImGui::Image((uint64_t)handle, ImGui::GetContentRegionAvail(), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

    ShowImGuizmoTransform();
    ShowImGuizmoOrientation();

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

}

void Viewport::ShowToolOverlay()
{
    // Display transform tools on the upper left corner
    constexpr size_t ToolCount = 4;
    constexpr std::array<int, ToolCount> Operations =
    {
        -1, // No ImGuizmo tramsform
        ImGuizmo::OPERATION::TRANSLATE,
        ImGuizmo::OPERATION::ROTATE,
        ImGuizmo::OPERATION::SCALE,
    };
    constexpr std::array<const char *, ToolCount> Icons =
    {
        ICON_MS_ARROW_SELECTOR_TOOL, ICON_MS_DRAG_PAN, ICON_MS_CACHED, ICON_MS_ZOOM_OUT_MAP,
    };

    auto ToolButton = [](size_t index)
    {
        const int op = Operations[index];
        ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, pData->m_imguizmoMode == op ? 1.0f : 0.5f);
        if (ImGui::Button(Icons[index], ImVec2{ 32.0f, 32.0f }))
        {
            pData->m_imguizmoMode = op;
        }
        ImGui::PopStyleVar();
    };

    ImGui::SetNextWindowPos(ImVec2{ (float)m_windowPosX + 16.0f, (float)m_windowPosY + 16.0f + GetTitleBarSize() });
    ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
    for (size_t i = 0; i < ToolCount; ++i)
    {
        ToolButton(i);
    }
    ImGui::End(); // Tools
}

void Viewport::ShowImGuizmoTransform()
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);
    if (pData->m_imguizmoMode < 0 || !pData->m_selectedEntity.IsValid() || pData->m_selectedEntity == sl::World::GetMainCameraEntity())
    {
        ImGuizmo::Enable(false);
        return;
    }

    // Disable ImGuizmo when camera moving
    auto &camera = sl::World::GetMainCameraComponent();
    ImGuizmo::Enable(!camera.IsUsing());

    ImGuizmo::AllowAxisFlip(false);
    ImGuizmo::SetOrthographic(camera.m_projectionType == sl::ProjectionType::Orthographic);
    ImGuizmo::SetRect((float)m_windowPosX, (float)m_windowPosY + GetTitleBarSize(), (float)m_windowSizeX, (float)m_windowSizeY);

    const glm::mat4 &view = camera.GetView();
    const glm::mat4 &projection = camera.GetProjection();
    auto &transform = pData->m_selectedEntity.GetComponents<sl::TransformComponent>();
    glm::mat4 manipulatedTransform = transform.GetTransform();

    ImGuizmo::SetDrawlist();
    ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
        (ImGuizmo::OPERATION)pData->m_imguizmoMode, ImGuizmo::LOCAL,
        glm::value_ptr(manipulatedTransform), nullptr, nullptr);

    if (ImGuizmo::IsUsing())
    {
        // Decompose transform mat to position, rotation and scale
        glm::vec3 newPosition, newRotation, newScale;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(manipulatedTransform),
            glm::value_ptr(newPosition), glm::value_ptr(newRotation), glm::value_ptr(newScale));

        transform.m_position = newPosition;
        transform.m_rotation += glm::radians(newRotation) - transform.m_rotation;
        transform.m_scale = newScale;
    }
}

void Viewport::ShowImGuizmoOrientation()
{
    // Display a cube on the upper right corner
    float length = 100.0f;
    ImVec2 pos = ImVec2{
        (float)m_windowPosX + (float)m_windowSizeX - length,
        (float)m_windowPosY + GetTitleBarSize() };

    auto view = sl::World::GetMainCameraComponent().GetView();
    ImGuizmo::ViewManipulate(glm::value_ptr(view), length, pos, ImVec2{ length , length }, 0);
}
