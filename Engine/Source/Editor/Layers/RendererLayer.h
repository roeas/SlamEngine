#pragma once

#include "Layer/Layer.h"

class RendererLayer : public sl::Layer
{
public:
    RendererLayer();
    ~RendererLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;

private:
    void BasePass();
    void EntityIDPass();
};
