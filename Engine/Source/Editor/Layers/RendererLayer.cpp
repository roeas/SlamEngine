#include "RendererLayer.h"

#include "Core/Path.h"
#include "Renderer/RenderCore.h"
#include "Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"
#include "Shader/Shared.h"
#include "Utils/ProfilerCPU.h"
#include "Utils/ProfilerGPU.h"

#include <glm/gtc/type_ptr.hpp>

namespace
{

constexpr sl::StringHashType CameraPosHash = sl::StringHash("ub_cameraPos");
constexpr sl::StringHashType ViewProjHash = sl::StringHash("ub_viewProjection");

void UploadMaterialPropertyGroup(sl::Shader *pShader, const auto &propertyGroup)
{
    if (propertyGroup.m_useTexture)
    {
        auto *pTextureResource = sl::ResourceManager::GetTextureResource(propertyGroup.m_textureID);
        if (pTextureResource && pTextureResource->IsReady())
        {
            pTextureResource->GetTexture()->Bind(propertyGroup.m_textureSlot);
            pShader->UploadUniform(propertyGroup.m_useTextureLocation, true);
        }
    }
    else
    {
        // sl::RenderCore::ClearTextureSlot(propertyGroup.m_textureSlot);
        pShader->UploadUniform(propertyGroup.m_useTextureLocation, false);
    }

    pShader->UploadUniform(propertyGroup.m_factorLocation, propertyGroup.m_factor);

    // TODO: UV transform
}

} // namespace

RendererLayer::RendererLayer()
{
    SL_PROFILE;

    // Main framebuffer and ID framebuffer, size is meaningless here
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

    // Camera uniform buffer
    sl::UniformBufferLayout cameraUniformBufferLayout;
    cameraUniformBufferLayout.AddElement(CameraPosHash, sl::UniformBufferLayoutElement{ 0, sizeof(glm::vec4) });
    cameraUniformBufferLayout.AddElement(ViewProjHash, sl::UniformBufferLayoutElement{ sizeof(glm::vec4), sizeof(glm::mat4) });
    cameraUniformBufferLayout.SetSize(sizeof(glm::vec4) + sizeof(glm::mat4));
    m_pCameraUniformBuffer.reset(sl::UniformBuffer::Create(SL_BINDING_POINT_CAMERA, std::move(cameraUniformBufferLayout)));

    // Shaders
    std::unique_ptr<sl::ShaderResource> pBaseShaderResource = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/Base_vert.glsl"), sl::Path::FromeAsset("Shader/Base_frag.glsl"));
    std::unique_ptr<sl::ShaderResource> pEntityIDShaderResource = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/EntityID_vert.glsl"), sl::Path::FromeAsset("Shader/EntityID_frag.glsl"));
    sl::ResourceManager::AddShaderResource(sl::StringHash("Base Shader"), std::move(pBaseShaderResource));
    sl::ResourceManager::AddShaderResource(sl::StringHash("EntityID Shader"), std::move(pEntityIDShaderResource));

    // Textures
    std::unique_ptr<sl::TextureResource> pNoResourceTextureResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/NoResource.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(sl::StringHash("NoResource Texture"), std::move(pNoResourceTextureResource));

    std::unique_ptr<sl::TextureResource> pDebugUVTextureResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/DebugUV.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(sl::StringHash("DebugUV Texture"), std::move(pDebugUVTextureResource));
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

    // State
    sl::RenderCore::DepthTestFunction(sl::Function::LessEqual);
    sl::RenderCore::BlendFunction(sl::Factor::SourceAlpha, sl::Factor::OneMinusSourceAlpha);
    sl::RenderCore::CullingFace(sl::Face::Back);

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
        auto *pMaterialResource = sl::ResourceManager::GetMaterialResource(rendering.m_materialResourceID);
        if (!pMeshResource || !pShaderResource || !pMaterialResource ||
            !pMeshResource->IsReady() || !pShaderResource->IsReady() || !pMaterialResource->IsReady())
        {
            continue;
        }

        auto *pShader = pShaderResource->GetShaderProgram();
        pShader->Bind();

        // Model mat
        glm::mat4 modelMat = transform.GetTransform();
        pShader->UploadUniform(SL_LOCATION_MODEL, modelMat);
        pShader->UploadUniform(SL_LOCATION_MODEL_INV_TRANS, glm::transpose(glm::inverse(modelMat)));

        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetAlbedoPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetNormalPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetOcclusionPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetRoughnessPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetMetallicPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetEmissivePropertyGroup());

        if (pMaterialResource->GetTwoSide())
        {
            sl::RenderCore::Culling(false);
        }

        pShader->Unbind();
        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShader);
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
        auto *pMaterialResource = sl::ResourceManager::GetMaterialResource(rendering.m_materialResourceID);
        if (!pMeshResource || !pShaderResource || !pMaterialResource ||
            !pMeshResource->IsReady() || !pShaderResource->IsReady() || !pMaterialResource->IsReady())
        {
            continue;
        }

        auto *pShader = pShaderResource->GetShaderProgram();
        pShader->Bind();
        
        pShader->UploadUniform(0, transform.GetTransform());
        pShader->UploadUniform(1, (int)entity);

        if (pMaterialResource->GetTwoSide())
        {
            sl::RenderCore::Culling(false);
        }

        pShader->Unbind();

        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShaderResource->GetShaderProgram());
    }

    sl::RenderCore::GetEntityIDFramebuffer()->Unbind();
}
