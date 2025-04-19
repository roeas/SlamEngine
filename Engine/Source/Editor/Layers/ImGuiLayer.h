#pragma once

#include "Layer/LayerStack.h"
#include "Panels/ImGuiData.h"

namespace sl
{

class KeyDownEvent;
class MouseButtonDownEvent;
class MouseButtonUpEvent;

} // namespace sl

class ImGuiLayer : public sl::Layer
{
public:
    ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;
    void SetEventCallback(auto fun) { m_eventCallback = fun; }
    void SetMainWindow(void *pWindow) { m_pMainWindow = pWindow; }

private:
    bool OnKeyDownEvent(sl::KeyDownEvent &event);
    bool OnMouseButtonDown(sl::MouseButtonDownEvent &event);
    bool OnMouseUpDown(sl::MouseButtonUpEvent &event);

    sl::EventCallback m_eventCallback;

    sl::LayerStack m_stack;
    void *m_pMainWindow;
    ImGuiData m_data;
};
