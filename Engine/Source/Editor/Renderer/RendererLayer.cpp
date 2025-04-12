#include "RendererLayer.h"

#include "Core/Path.h"
#include "Renderer/RenderCore.h"
#include "Scene/World.h"
#include "Utils/NameOf.hpp"

#include <glm/gtc/type_ptr.hpp>

RendererLayer::RendererLayer()
{

}

void RendererLayer::OnAttach()
{

}

void RendererLayer::OnDetach()
{

}

void RendererLayer::OnEvent(sl::Event &event)
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

void RendererLayer::BasePass()
{
    // TODO
}

void RendererLayer::EntityIDPass()
{
    // TODO
}
