#include "RendererLayer.h"

#include "Renderer/RenderCore.h"
#include "Scene/World.h"

#include <glm/gtc/type_ptr.hpp>

RendererLayer::RendererLayer()
{

}

RendererLayer::~RendererLayer()
{

}

void RendererLayer::OnAttach()
{

}

void RendererLayer::OnDetach()
{

}

void RendererLayer::BeginFrame()
{

}

void RendererLayer::OnUpdate(float deltaTime)
{

}

void RendererLayer::OnRender()
{
    // Upload camera uniform buffer
    if (auto *pCameraUniformBuffer = sl::RenderCore::GetUniformBuffer("CameraUniformBuffer"); pCameraUniformBuffer)
    {
        sl::Entity mainCamera = sl::World::GetMainCameraEntity();
        pCameraUniformBuffer->Upload("ub_cameraPos", glm::value_ptr(mainCamera.GetComponents<sl::TransformComponent>().m_position));
        pCameraUniformBuffer->Upload("ub_viewProjection", glm::value_ptr(mainCamera.GetComponents<sl::CameraComponent>().GetViewProjection()));
    }

    BasePass();
    EntityIDPass();
}

void RendererLayer::EndFrame()
{

}

void RendererLayer::OnEvent(sl::Event &event)
{

}

void RendererLayer::BasePass()
{
    sl::RenderCore::GetMainFramebuffer()->Bind();
    sl::RenderCore::ClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    sl::RenderCore::ClearDepth(1.0f);

    auto group = sl::World::GetRegistry().group<sl::RenderingComponent, sl::TransformComponent>();
    for (auto entity : group)
    {
        auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
        rendering.m_pShader->Bind();
        rendering.m_pShader->UploadUniform(0, transform.GetTransform());
        rendering.m_pTexture->Bind(0);
        rendering.m_pShader->Unbind();

        sl::RenderCore::Submit(rendering.m_pVertexArray, rendering.m_pShader);
    }
 
    sl::RenderCore::GetMainFramebuffer()->Unbind();
}

void RendererLayer::EntityIDPass()
{
    constexpr int entityIDClearData = -1;
    sl::RenderCore::GetEntityIDFramebuffer()->Bind();
    sl::RenderCore::GetEntityIDFramebuffer()->Clear(0, &entityIDClearData);
    sl::RenderCore::ClearDepth(1.0f);

    auto group = sl::World::GetRegistry().group<sl::RenderingComponent, sl::TransformComponent>();
    for (auto entity : group)
    {
        auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);

        rendering.m_pIDShader->Bind();
        rendering.m_pIDShader->UploadUniform(0, transform.GetTransform());
        rendering.m_pIDShader->UploadUniform(1, (int)entity);
        rendering.m_pIDShader->Unbind();

        sl::RenderCore::Submit(rendering.m_pVertexArray, rendering.m_pIDShader);
    }

    sl::RenderCore::GetEntityIDFramebuffer()->Unbind();
}
