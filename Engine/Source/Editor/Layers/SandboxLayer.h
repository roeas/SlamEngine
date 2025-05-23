#pragma once

#include "Layer/Layer.h"

class SandboxLayer : public sl::Layer
{
public:
    SandboxLayer();

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;
};
