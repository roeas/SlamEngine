#pragma once

#include "Layer/Layer.h"

namespace sl
{

class MouseScrollEvent;

}

class CameraControllerLayer : public sl::Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(sl::Event &event) override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

private:
    void UpdateFPSMode(float deltaTime);
    void UpdateEditorMode(float deltaTime);

    bool OnMouseScroll(sl::MouseScrollEvent &event);
};
