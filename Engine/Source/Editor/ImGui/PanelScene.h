#pragma once

#include "Layer/Layer.h"

class PanelScene : public sl::Layer
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
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};
