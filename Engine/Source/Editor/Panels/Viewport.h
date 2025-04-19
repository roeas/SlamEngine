#pragma once

#include "Layer/Layer.h"

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
    void ShowToolOverlay();
    void ShowImGuizmoTransform();
    void ShowImGuizmoOrientation();

    uint32_t m_windowPosX = 0;
    uint32_t m_windowPosY = 0;
    uint32_t m_windowSizeX = 0;
    uint32_t m_windowSizeY = 0;
};
