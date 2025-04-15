#include "Panel/Details.h"

#include <imgui/imgui.h>

void Details::OnAttach()
{

}

void Details::OnDetach()
{

}

void Details::BeginFrame()
{

}

void Details::OnUpdate(float deltaTime)
{
    if (!ImGui::Begin("Details"))
    {
        ImGui::End();
        return;
    }

    ImGui::End();
}

void Details::OnRender()
{

}

void Details::EndFrame()
{

}

void Details::OnEvent(sl::Event &event)
{

}
