#pragma once

#include "Layer/Layer.h"

class RendererLayer : public sl::Layer
{
public:
    RendererLayer();
    ~RendererLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(sl::Event &event) override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

private:
    void BasePass();
    void EntityIDPass();
};
