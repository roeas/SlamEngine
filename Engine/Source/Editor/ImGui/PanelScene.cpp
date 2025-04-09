#include "ImGui/PanelScene.h"

#include "Renderer/RenderCore.h"

#include <imgui/imgui.h>

void PanelScene::OnAttach()
{

}

void PanelScene::OnDetach()
{

}

void PanelScene::BeginFrame()
{

}

void PanelScene::OnUpdate(float deltaTime)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 0.0f });
    ImGui::Begin("Scene");
    ImGui::PopStyleVar();

    ImVec2 size = ImGui::GetContentRegionAvail();
    if (m_width != size.x || m_height != size.y)
    {
        m_width = (uint32_t)size.x;
        m_height = (uint32_t)size.y;
        sl::RenderCore::GetMainFramebuffer()->Resize(m_width, m_height);
    }

    // Draw main frame buffer color attachment
    uint32_t handle = sl::RenderCore::GetMainFramebuffer()->GetAttachmentHandle(0);
    ImGui::Image((uint64_t)handle, ImGui::GetContentRegionAvail(), ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f });

    ImGui::End();
}

void PanelScene::OnRender()
{

}

void PanelScene::EndFrame()
{

}

void PanelScene::OnEvent(sl::Event &event)
{

}
