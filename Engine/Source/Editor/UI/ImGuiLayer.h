#pragma once

#include "Layer/Layer.h"

class ImGuiLayer : public sl::Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer() override;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(sl::Event &event) override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;
};
