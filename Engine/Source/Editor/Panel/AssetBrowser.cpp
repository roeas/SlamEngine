#include "Panel/AssetBrowser.h"

#include <imgui/imgui.h>

void AssetBrowser::OnAttach()
{

}

void AssetBrowser::OnDetach()
{

}

void AssetBrowser::BeginFrame()
{

}

void AssetBrowser::OnUpdate(float deltaTime)
{
    if (!ImGui::Begin("Asset Browser"))
    {
        ImGui::End();
        return;
    }

    ImGui::End();
}

void AssetBrowser::OnRender()
{

}

void AssetBrowser::EndFrame()
{

}

void AssetBrowser::OnEvent(sl::Event &event)
{

}
