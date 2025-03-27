#pragma once

#include "ImGui/PanelEntityList.h"

#include <imgui/imgui.h>

void PanelEntityList::OnAttach()
{

}

void PanelEntityList::OnDetach()
{

}

void PanelEntityList::BeginFrame()
{

}

void PanelEntityList::OnUpdate(float deltaTime)
{
    ImGui::Begin("Entity List");

    ImGui::End();
}

void PanelEntityList::OnRender()
{

}

void PanelEntityList::EndFrame()
{

}

void PanelEntityList::OnEvent(sl::Event &event)
{

}
