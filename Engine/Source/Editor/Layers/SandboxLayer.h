#pragma once

#include "Layer/Layer.h"

namespace sl
{

class Shader;
class Texture2D;
class VertexArray;

} // namespace sl

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
    std::unique_ptr<sl::VertexArray> m_pVertexArray;
    std::unique_ptr<sl::Texture2D> m_pTexture;
    std::unique_ptr<sl::Shader> m_pShader;
};
