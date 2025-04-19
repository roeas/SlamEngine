#pragma once

#include "Layer/Layer.h"

class Details : public sl::Layer
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
    float StartWithText(std::string_view text, float offset = 0.0f);

    float m_maxTextSize = 56.0f;
};
