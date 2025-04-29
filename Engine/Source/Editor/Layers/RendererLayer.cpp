#include "RendererLayer.h"

#include "Renderer/RenderCore.h"
#include "Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"
#include "Utils/ProfilerGPU.h"

#include <glm/gtc/type_ptr.hpp>

namespace
{

constexpr sl::StringHashType CameraPosHash = sl::StringHash("ub_cameraPos");
constexpr sl::StringHashType ViewProjHash = sl::StringHash("ub_viewProjection");

} // namespace

RendererLayer::RendererLayer()
{
    SL_PROFILE;

    // Create main framebuffer and ID framebuffer, size is meaningless here
    sl::RenderCore::SetMainFramebuffer(sl::Framebuffer::Create(
    {
        sl::Texture2D::Create(1, 1, sl::TextureFormat::RGB8, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
        sl::Texture2D::Create(1, 1, sl::TextureFormat::D32, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
    }));
    sl::RenderCore::SetEntityIDFramebuffer(sl::Framebuffer::Create(
    {
        sl::Texture2D::Create(1, 1, sl::TextureFormat::R32I, false, SL_SAMPLER_CLAMP | SL_SAMPLER_NEAREST),
        sl::Texture2D::Create(1, 1, sl::TextureFormat::D32, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
    }));

    // Create camera uniform buffer
    sl::UniformBufferLayout cameraUniformBufferLayout;
    cameraUniformBufferLayout.AddElement(CameraPosHash, sl::UniformBufferLayoutElement{ 0, sizeof(glm::vec4) });
    cameraUniformBufferLayout.AddElement(ViewProjHash, sl::UniformBufferLayoutElement{ sizeof(glm::vec4), sizeof(glm::mat4) });
    cameraUniformBufferLayout.SetSize(sizeof(glm::vec4) + sizeof(glm::mat4));
    m_pCameraUniformBuffer.reset(sl::UniformBuffer::Create(0, std::move(cameraUniformBufferLayout)));
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
    SL_PROFILE;

    // Upload camera uniform buffer
    sl::Entity mainCamera = sl::World::GetMainCameraEntity();
    m_pCameraUniformBuffer->Upload(CameraPosHash, glm::value_ptr(mainCamera.GetComponents<sl::TransformComponent>().m_position));
    m_pCameraUniformBuffer->Upload(ViewProjHash, glm::value_ptr(mainCamera.GetComponents<sl::CameraComponent>().GetViewProjection()));

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
    SL_PROFILE_GPU("Base Pass");
    SL_PROFILE;

    sl::RenderCore::GetMainFramebuffer()->Bind();
    sl::RenderCore::ClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    sl::RenderCore::ClearDepth(1.0f);

    auto group = sl::World::GetRegistry().group<sl::RenderingComponent, sl::TransformComponent>();
    for (auto entity : group)
    {
        auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);

        auto *pMeshResource = sl::ResourceManager::GetMeshResource(rendering.m_meshResourceID);
        auto *pShaderResource = sl::ResourceManager::GetShaderResource(rendering.m_shaderResourceID);
        if (!pMeshResource || !pShaderResource || !pMeshResource->IsReady() || !pShaderResource->IsReady())
        {
            continue;
        }

        pShaderResource->GetShaderProgram()->Bind();
        pShaderResource->GetShaderProgram()->UploadUniform(0, transform.GetTransform());

        auto *pTextureResource = sl::ResourceManager::GetTextureResource(rendering.m_textureResourceID);
        if (pTextureResource && pTextureResource->IsReady())
        {
            pTextureResource->GetTexture()->Bind(0);
        }

        pShaderResource->GetShaderProgram()->Unbind();
        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShaderResource->GetShaderProgram());
    }
 
    sl::RenderCore::GetMainFramebuffer()->Unbind();
}

void RendererLayer::EntityIDPass()
{
    SL_PROFILE_GPU("Entity ID Pass");
    SL_PROFILE;

    constexpr int entityIDClearData = -1;
    sl::RenderCore::GetEntityIDFramebuffer()->Bind();
    sl::RenderCore::GetEntityIDFramebuffer()->Clear(0, &entityIDClearData);
    sl::RenderCore::ClearDepth(1.0f);

    auto group = sl::World::GetRegistry().group<sl::RenderingComponent, sl::TransformComponent>();
    for (auto entity : group)
    {
        auto [rendering, transform] = group.get<sl::RenderingComponent, sl::TransformComponent>(entity);

        auto *pMeshResource = sl::ResourceManager::GetMeshResource(rendering.m_meshResourceID);
        auto *pShaderResource = sl::ResourceManager::GetShaderResource(rendering.m_entityIDShaderResourceID);
        if (!pMeshResource || !pShaderResource || !pMeshResource->IsReady() || !pShaderResource->IsReady())
        {
            continue;
        }

        pShaderResource->GetShaderProgram()->Bind();
        pShaderResource->GetShaderProgram()->UploadUniform(0, transform.GetTransform());
        pShaderResource->GetShaderProgram()->UploadUniform(1, (int)entity);
        pShaderResource->GetShaderProgram()->Unbind();

        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShaderResource->GetShaderProgram());
    }

    sl::RenderCore::GetEntityIDFramebuffer()->Unbind();
}
