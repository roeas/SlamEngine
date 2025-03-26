#pragma once

#include "ImGui/MenuBar.h"

#include "Core/Log.h"
#include "Event/WindowEvent.h"
#include "ImGui/ImGuiData.h"

#include <imgui/imgui.h>

void MenuBar::OnAttach()
{

}

void MenuBar::OnDetach()
{

}

void MenuBar::BeginFrame()
{

}

void MenuBar::OnUpdate(float deltaTime)
{
    ImGuiData *pData = static_cast<ImGuiData *>(ImGui::GetIO().UserData);

    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New"))
        {

        }
        if (ImGui::MenuItem("Open"))
        {

        }
        if (ImGui::MenuItem("Save"))
        {

        }

        ImGui::Separator();
        
        if (ImGui::MenuItem("Exit"))
        {
            sl::WindowCloseEvent event;
            m_eventCallback(event);
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Setting"))
    {
        if (ImGui::MenuItem("Dockspace Node Undockable", "", !(pData->m_dockspaceFlag & ImGuiDockNodeFlags_NoUndocking)))
        {
            pData->m_dockspaceFlag ^= ImGuiDockNodeFlags_NoUndocking;
        }
        if (ImGui::MenuItem("Dockspace Node Resizable", "", !(pData->m_dockspaceFlag & ImGuiDockNodeFlags_NoResize)))
        {
            pData->m_dockspaceFlag ^= ImGuiDockNodeFlags_NoResize;
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Debug"))
    {
        ImGui::MenuItem("ImGui Demo", "", &pData->m_debugImGuiDemo);
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void MenuBar::OnRender()
{

}

void MenuBar::EndFrame()
{

}

void MenuBar::OnEvent(sl::Event &event)
{

}
