#pragma once

#include "Layer/LayerStack.h"
#include "ImGui/ImGuiData.h"

class ImGuiLayer : public sl::Layer
{
public:
    ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;
    void SetEventCallback(auto fun) { m_eventCallback = fun; }
    void ForwardEvent(sl::Event &event);

private:
    sl::EventCallback m_eventCallback;

    sl::LayerStack m_stack;
    ImGuiData m_data;
};
