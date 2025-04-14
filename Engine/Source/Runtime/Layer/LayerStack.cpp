#include "LayerStack.h"

namespace sl
{

void LayerStack::PushLayer(std::unique_ptr<Layer> pLayer)
{
    pLayer->OnAttach();
    m_pLayers.push_back(std::move(pLayer));
}

void LayerStack::BeginFrame()
{
    for (auto &pLayer : m_pLayers)
    {
        pLayer->BeginFrame();
    }
}

void LayerStack::OnUpdate(float deltaTime)
{
    for (auto &pLayer : m_pLayers)
    {
        pLayer->OnUpdate(deltaTime);
    }
}

void LayerStack::OnRender()
{
    for (auto &pLayer : m_pLayers)
    {
        pLayer->OnRender();
    }
}

void LayerStack::EndFrame()
{
    for (auto &pLayer : m_pLayers)
    {
        pLayer->EndFrame();
    }
}

void LayerStack::OnEvent(Event &event)
{
    // Iterate layers from top to bottom / from end to begin
    for (auto it = rbegin(); it != rend(); ++it)
    {
        if (event.IsHandled())
        {
            break;
        }
        (*it)->OnEvent(event);
    }
}

} // namespace sl
