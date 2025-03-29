#pragma once

#include "Layer/Layer.h"

namespace sl
{

class Shader;

}

class SandboxLayer : public sl::Layer
{
public:
    SandboxLayer();
    ~SandboxLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    void BeginFrame() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void EndFrame() override;

    void OnEvent(sl::Event &event) override;

private:
    uint32_t m_vb, m_ib, m_va;
    std::unique_ptr<sl::Shader> m_pShader;
};
