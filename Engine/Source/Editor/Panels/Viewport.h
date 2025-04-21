#pragma once

#include "Layer/Layer.h"

namespace sl
{

class MouseButtonDownEvent;

} // namespace sl

class Viewport : public sl::Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;

private:
    bool OnMouseButtonDown(sl::MouseButtonDownEvent &event);
    bool MousePick();

    void ShowImGuizmoTransform();
    void ShowImGuizmoOrientation();
    void ShowToolOverlay();

    uint32_t m_windowPosX = 0;
    uint32_t m_windowPosY = 0;
    uint32_t m_windowSizeX = 0;
    uint32_t m_windowSizeY = 0;
};
