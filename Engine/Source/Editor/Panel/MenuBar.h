#pragma once

#include "Layer/Layer.h"

class MenuBar : public sl::Layer
{
public:
    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;
    void SetEventCallback(auto fun) { m_eventCallback = fun; }

private:
    sl::EventCallback m_eventCallback;
};
