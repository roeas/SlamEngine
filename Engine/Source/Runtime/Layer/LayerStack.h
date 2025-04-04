#pragma once

#include "Layer/Layer.h"

#include <memory>
#include <vector>

namespace sl
{

class LayerStack final
{
public:
    void PushLayer(std::unique_ptr<Layer> pLayer);

    void BeginFrame();
    void OnUpdate(float deltaTime);
    void OnRender();
    void EndFrame();

    void OnEvent(Event &event);

    auto begin() { return m_pLayers.begin(); }
    auto end() { return m_pLayers.end(); }
    auto cbegin() const { return m_pLayers.cbegin(); }
    auto cend() const { return m_pLayers.cend(); }

    auto rbegin() { return m_pLayers.rbegin(); }
    auto rend() { return m_pLayers.rend(); }
    auto crbegin() const { return m_pLayers.crbegin(); }
    auto crend() const { return m_pLayers.crend(); }

private:
    std::vector<std::unique_ptr<Layer>> m_pLayers;
};

} // namespace sl
