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

void LayerStack::Update(float deltaTime)
{
    for (auto &pLayer : m_pLayers)
    {
        pLayer->OnUpdate(deltaTime);
    }
}

void LayerStack::Render()
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

} // namespace sl
