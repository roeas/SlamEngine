#pragma once

#include "ImGui/PanelScene.h"

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
    ImGui::Begin("Scene");

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
