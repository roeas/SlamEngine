#pragma once

#include "Layer/Layer.h"

#include <memory>

namespace sl
{

class UniformBuffer;

} // namespace sl

class RendererLayer : public sl::Layer
{
public:
    RendererLayer();
    ~RendererLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;

    void SetCameraUniformBuffer(std::unique_ptr<sl::UniformBuffer> pUniformBuffer) { m_pCameraUniformBuffer = std::move(pUniformBuffer); }

private:
    void BasePass();
    void EntityIDPass();

    std::unique_ptr<sl::UniformBuffer> m_pCameraUniformBuffer;
};
