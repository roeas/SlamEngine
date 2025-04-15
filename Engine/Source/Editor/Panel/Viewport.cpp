#include "Panel/Viewport.h"

#include "Renderer/RenderCore.h"
#include "Scene/World.h"

#include <imgui/imgui.h>

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
    ImGui::Begin("Scene");
    ImGui::PopStyleVar();

    ImVec2 size = ImGui::GetContentRegionAvail();
    if (m_width != size.x || m_height != size.y)
    {
        m_width = (uint32_t)size.x;
        m_height = (uint32_t)size.y;

        // Resize main framebuffer
        sl::RenderCore::GetMainFramebuffer()->Resize(m_width, m_height);

        // Update main camera aspect
        float aspect = (float)m_width / (float)m_height;
        auto &camera = sl::World::GetMainCameraComponent();
        camera.m_aspect = aspect;
        camera.m_fovMultiplier = aspect * 9.0f / 16.0f;
        camera.m_isDirty = true;
    }

    // Draw main frame buffer color attachment
    uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
    ImGui::Image((uint64_t)handle, ImGui::GetContentRegionAvail(), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

    ImGui::End();
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
