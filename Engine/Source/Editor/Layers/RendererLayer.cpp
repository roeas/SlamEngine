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
    sl::RenderCore::SetClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    sl::RenderCore::Clear();

    auto group = sl::World::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
    for (auto entity : group)
    {
        auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);
        rendering.m_pShader->Bind();
        glm::mat4 modelMat = transform.GetTransform();
        rendering.m_pShader->UploadUniform(0, modelMat);
        rendering.m_pTexture->Bind(0);
        rendering.m_pShader->Unbind();

        sl::RenderCore::Submit(rendering.m_pVertexArray, rendering.m_pShader);
    }
 
    sl::RenderCore::GetMainFramebuffer()->Unbind();
}

void RendererLayer::EntityIDPass()
{
    // TODO
}
