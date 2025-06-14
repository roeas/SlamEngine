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

constexpr sl::StringHashType CameraPosID = sl::StringHash("ub_cameraPos");
constexpr sl::StringHashType ViewMatID = sl::StringHash("ub_viewMat");
constexpr sl::StringHashType ProjectionMatID = sl::StringHash("ub_projectionMat");
constexpr sl::StringHashType ViewProjectionMatID = sl::StringHash("ub_viewProjctionMat");
constexpr sl::StringHashType ViewMatWithoutTransformID = sl::StringHash("ub_viewMatWithoutTransform");
constexpr sl::StringHashType LightsID = sl::StringHash("ub_lights");
constexpr sl::StringHashType LightCountID = sl::StringHash("ub_lightCount");

constexpr sl::StringHashType RadianceTextureID = sl::StringHash("Radiance Texture");
constexpr sl::StringHashType IrradianceTextureID = sl::StringHash("Irradiance Texture");
constexpr sl::StringHashType IBLLUTTextureID = sl::StringHash("IBL LUT Texture");

void UploadMaterialPropertyGroup(sl::Shader *pShader, const auto &propertyGroup)
{
    auto *pTextureResource = sl::ResourceManager::GetTextureResource(propertyGroup.m_textureID);
    if (propertyGroup.m_useTexture && pTextureResource && pTextureResource->IsReady())
    {
        pTextureResource->GetTexture()->Bind(propertyGroup.m_textureSlot);
        pShader->UploadUniform(propertyGroup.m_useTextureLocation, true);
    }
    else
    {
        sl::RenderCore::ClearTextureSlot(propertyGroup.m_textureSlot);
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
        sl::Texture2D::Create(1, 1, sl::TextureFormat::RGB32F, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
        sl::Texture2D::Create(1, 1, sl::TextureFormat::D32, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
    }));
    sl::RenderCore::SetFinalFramebuffer(sl::Framebuffer::Create(
    {
        sl::Texture2D::Create(1, 1, sl::TextureFormat::RGB8, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
    }));
    sl::RenderCore::SetEntityIDFramebuffer(sl::Framebuffer::Create(
    {
        sl::Texture2D::Create(1, 1, sl::TextureFormat::R32I, false, SL_SAMPLER_CLAMP | SL_SAMPLER_NEAREST),
        sl::Texture2D::Create(1, 1, sl::TextureFormat::D32, false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR),
    }));

    // Camera uniform buffer
    uint32_t cameraUBSize = 0;
    sl::UniformBufferLayout cameraUBayout;
    cameraUBayout.AddElement(CameraPosID, sl::UniformBufferLayoutElement{ cameraUBSize, sizeof(glm::vec4) });
    cameraUBSize += sizeof(glm::vec4);
    cameraUBayout.AddElement(ViewMatID, sl::UniformBufferLayoutElement{ cameraUBSize, sizeof(glm::mat4) });
    cameraUBSize += sizeof(glm::mat4);
    cameraUBayout.AddElement(ProjectionMatID, sl::UniformBufferLayoutElement{ cameraUBSize, sizeof(glm::mat4) });
    cameraUBSize += sizeof(glm::mat4);
    cameraUBayout.AddElement(ViewProjectionMatID, sl::UniformBufferLayoutElement{ cameraUBSize, sizeof(glm::mat4) });
    cameraUBSize += sizeof(glm::mat4);
    cameraUBayout.AddElement(ViewMatWithoutTransformID, sl::UniformBufferLayoutElement{ cameraUBSize, sizeof(glm::mat4) });
    cameraUBSize += sizeof(glm::mat4);
    cameraUBayout.SetSize(cameraUBSize);
    m_pCameraUniformBuffer.reset(sl::UniformBuffer::Create(SL_BINDING_POINT_CAMERA, std::move(cameraUBayout)));

    // Light uniform buffer
    uint32_t lightUBize = 0;
    sl::UniformBufferLayout lightUBLayout;
    lightUBLayout.AddElement(LightsID, sl::UniformBufferLayoutElement{ lightUBize, sizeof(SL_LightUniformBuffer) * SL_LIGHT_MAX_COUNT });
    lightUBize += sizeof(SL_LightUniformBuffer) * SL_LIGHT_MAX_COUNT;
    lightUBLayout.AddElement(LightCountID, sl::UniformBufferLayoutElement{ lightUBize, sizeof(int32_t) });
    lightUBize += sizeof(int32_t);
    lightUBLayout.SetSize(lightUBize);
    m_pLightUniformBuffer.reset(sl::UniformBuffer::Create(SL_BINDING_POINT_LIGHT, std::move(lightUBLayout)));

    // IBL texture
    std::unique_ptr<sl::TextureResource> pRadiance = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/Rad.ktx"), false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR);
    std::unique_ptr<sl::TextureResource> pIrradiance = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/Irr.ktx"), false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR);
    std::unique_ptr<sl::TextureResource> pIBLLUT = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/DFG.png"), false, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(RadianceTextureID, std::move(pRadiance));
    sl::ResourceManager::AddTextureResource(IrradianceTextureID, std::move(pIrradiance));
    sl::ResourceManager::AddTextureResource(IBLLUTTextureID, std::move(pIBLLUT));
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
    auto &camera = mainCamera.GetComponents<sl::CameraComponent>();
    m_pCameraUniformBuffer->Upload(CameraPosID, glm::value_ptr(mainCamera.GetComponents<sl::TransformComponent>().m_position));
    m_pCameraUniformBuffer->Upload(ViewMatID, glm::value_ptr(camera.GetView()));
    m_pCameraUniformBuffer->Upload(ProjectionMatID, glm::value_ptr(camera.GetProjection()));
    m_pCameraUniformBuffer->Upload(ViewProjectionMatID, glm::value_ptr(camera.GetViewProjection()));
    m_pCameraUniformBuffer->Upload(ViewMatWithoutTransformID, glm::value_ptr(glm::mat4{ glm::mat3{ camera.GetView() } }));

    // Upload light uniform buffer
    static std::vector<SL_LightUniformBuffer> s_lightSharedData(SL_LIGHT_MAX_COUNT);
    s_lightSharedData.clear();
    auto group = sl::World::GetRegistry().group<sl::LightComponent>(entt::get<sl::TransformComponent>);
    int lightCount = (int)group.size();
    for (auto entity : group)
    {
        auto [light, transform] = group.get<sl::LightComponent, sl::TransformComponent>(entity);
        SL_LightUniformBuffer lightData;
        lightData.type = (int)light.m_type;
        lightData.intensity = light.m_intensity;
        lightData.range = light.m_range;

        float cosInner = std::cos(light.m_inner);
        float cosOuter = std::cos(light.m_outer);
        lightData.scale = 1.0f / std::max(cosInner - cosOuter, 0.001f);
        lightData.offset = -cosOuter * lightData.scale;

        memcpy(&lightData.colorR, &light.m_color, sizeof(float) * 3);
        memcpy(&lightData.positionX, &transform.m_position, sizeof(float) * 3);
        glm::vec3 dir = glm::normalize(glm::vec3{ transform.GetRotate() * glm::vec4{ 0.0f, 0.0f, -1.0f, 0.0f } });
        memcpy(&lightData.directionX, &dir, sizeof(float) * 3);

        s_lightSharedData.push_back(lightData);
    }

    m_pLightUniformBuffer->Upload(LightsID, s_lightSharedData.data(), sizeof(SL_LightUniformBuffer) * lightCount);
    m_pLightUniformBuffer->Upload(LightCountID, &lightCount);

    ClearMainFramebuffer();
    SkyPass();
    StandardPass();
    PostProcessingPass();
    EntityIDPass();
}

void RendererLayer::EndFrame()
{

}

void RendererLayer::OnEvent(sl::Event &event)
{

}

void RendererLayer::ClearMainFramebuffer()
{
    auto pFramebuffer = sl::RenderCore::GetMainFramebuffer();
    pFramebuffer->Bind();
    sl::RenderCore::ClearColor(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
    sl::RenderCore::ClearDepth(1.0f);
    pFramebuffer->Unbind();
}

void RendererLayer::SkyPass()
{
    SL_PROFILE_GPU("Sky Pass");
    SL_PROFILE;

    sl::RenderCore::Culling(false);
    sl::RenderCore::DepthTest(false);
    sl::RenderCore::SeamlessCubemap(true);

    auto pFramebuffer = sl::RenderCore::GetMainFramebuffer();
    pFramebuffer->Bind();

    auto view = sl::World::GetRegistry().view<sl::SkyComponent>();
    for (auto entity : view)
    {
        auto &sky = view.get<sl::SkyComponent>(entity);
        auto *pMeshResource = sl::ResourceManager::GetMeshResource(sky.m_meshResourceID);
        auto *pShaderResource = sl::ResourceManager::GetShaderResource(sky.m_shaderResourceID);
        auto *pTextureResource = sl::ResourceManager::GetTextureResource(sky.m_textureResourceID);
        if (!pMeshResource || !pShaderResource || !pTextureResource ||
            !pMeshResource->IsReady() || !pShaderResource->IsReady() || !pTextureResource->IsReady())
        {
            continue;
        }

        auto *pShader = pShaderResource->GetShaderProgram();
        pShader->Bind();
        pShader->UploadUniform(SL_LOCATION_SKY_FACTOR, sky.m_factor);
        pTextureResource->GetTexture()->Bind(0);
        pShader->Unbind();

        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShaderResource->GetShaderProgram());
    }

    pFramebuffer->Unbind();
}

void RendererLayer::StandardPass()
{
    SL_PROFILE_GPU("Standard Pass");
    SL_PROFILE;

    sl::RenderCore::CullingFace(sl::Face::Back);
    sl::RenderCore::DepthTestFunction(sl::Function::Less);
    sl::RenderCore::BlendFunction(sl::Factor::SourceAlpha, sl::Factor::OneMinusSourceAlpha);
    sl::RenderCore::SeamlessCubemap(true);

    auto pFramebuffer = sl::RenderCore::GetMainFramebuffer();
    pFramebuffer->Bind();

    // Sort entities, first render opaque materials from near to far, then render transparent materials from far to near
    const glm::vec3 &cameraPos = sl::World::GetMainCameraTransformComponent().m_position;
    auto group = sl::World::GetRegistry().group<sl::RenderingComponent>(entt::get<sl::TransformComponent>);
    group.sort([&group, &cameraPos](const entt::entity l, const entt::entity r)
    {
        auto *pMaterialL = sl::ResourceManager::GetMaterialResource(group.get<sl::RenderingComponent>(l).m_materialResourceID);
        auto *pMaterialR = sl::ResourceManager::GetMaterialResource(group.get<sl::RenderingComponent>(r).m_materialResourceID);
        bool isTransparentL = pMaterialL->GetRenderingMode() == sl::RenderingMode::Transparent;
        bool isTransparentR = pMaterialR->GetRenderingMode() == sl::RenderingMode::Transparent;
        if (isTransparentL != isTransparentR)
        {
            return !isTransparentL;
        }

        float distLhs = glm::distance(cameraPos, group.get<sl::TransformComponent>(l).m_position);
        float distRhs = glm::distance(cameraPos, group.get<sl::TransformComponent>(r).m_position);
        return isTransparentL ? (distLhs > distRhs) : (distLhs < distRhs);
    });

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
        pShader->UploadUniform(SL_LOCATION_RENDERING_MODE, (int)pMaterialResource->GetRenderingMode());
        pShader->UploadUniform(SL_LOCATION_ALPHA_CUTOFF, pMaterialResource->GetAlphaCutoff());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetAlbedoPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetNormalPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetOcclusionPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetRoughnessPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetMetallicPropertyGroup());
        UploadMaterialPropertyGroup(pShader, pMaterialResource->GetEmissivePropertyGroup());

        // IBL
        auto *pRadianceTextureResource = sl::ResourceManager::GetTextureResource(RadianceTextureID);
        auto *pIrradianceTextureResource = sl::ResourceManager::GetTextureResource(IrradianceTextureID);
        auto *pIBLLUTTextureResource = sl::ResourceManager::GetTextureResource(IBLLUTTextureID);
        if(pRadianceTextureResource && pIrradianceTextureResource && pIBLLUTTextureResource &&
            pRadianceTextureResource->IsReady() && pIrradianceTextureResource->IsReady() && pIBLLUTTextureResource->IsReady())
        {
            pRadianceTextureResource->GetTexture()->Bind(SL_SLOT_RADIANCE);
            pIrradianceTextureResource->GetTexture()->Bind(SL_SLOT_IRRADIANCE);
            pIBLLUTTextureResource->GetTexture()->Bind(SL_SLOT_IBL_LUT);
            
            pShader->UploadUniform(SL_LOCATION_IBL_FACTOR, pMaterialResource->GetIBLFactor());
        }

        pShader->Unbind();

        // States
        sl::RenderCore::Culling(!pMaterialResource->GetTwoSide());
        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShader);
    }
 
    pFramebuffer->Unbind();
}

void RendererLayer::PostProcessingPass()
{
    SL_PROFILE_GPU("Post Processing Pass");
    SL_PROFILE;

    sl::RenderCore::CullingFace(sl::Face::Back);

    auto pFramebuffer = sl::RenderCore::GetFinalFramebuffer();
    pFramebuffer->Bind();
    sl::RenderCore::ClearColor(glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });

    auto view = sl::World::GetRegistry().view<sl::PostProcessingComponent>();
    for (auto entity : view)
    {
        auto &postProcessing = view.get<sl::PostProcessingComponent>(entity);
        auto *pMeshResource = sl::ResourceManager::GetMeshResource(postProcessing.m_meshResourceID);
        auto *pShaderResource = sl::ResourceManager::GetShaderResource(postProcessing.m_shaderResourceID);
        if (!pMeshResource || !pShaderResource || !pMeshResource->IsReady() || !pShaderResource->IsReady())
        {
            continue;
        }

        auto *pShader = pShaderResource->GetShaderProgram();
        pShader->Bind();

        sl::RenderCore::GetMainFramebuffer()->GetAttachment(0)->Bind(0);
        pShader->UploadUniform(0, (int)postProcessing.m_toneMappingMode);

        pShader->Unbind();

        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShaderResource->GetShaderProgram());
    }

    pFramebuffer->Unbind();
}

void RendererLayer::EntityIDPass()
{
    SL_PROFILE_GPU("Entity ID Pass");
    SL_PROFILE;

    sl::RenderCore::CullingFace(sl::Face::Back);
    sl::RenderCore::DepthTestFunction(sl::Function::Less);

    auto pFramebuffer = sl::RenderCore::GetEntityIDFramebuffer();
    pFramebuffer->Bind();
    constexpr int entityIDClearData = -1;
    pFramebuffer->Clear(0, &entityIDClearData);
    sl::RenderCore::ClearDepth(1.0f);

    auto view = sl::World::GetRegistry().view<sl::RenderingComponent, sl::TransformComponent>();
    for (auto entity : view)
    {
        auto [rendering, transform] = view.get<sl::RenderingComponent, sl::TransformComponent>(entity);
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

        sl::RenderCore::Culling(!pMaterialResource->GetTwoSide());
        pShader->Unbind();

        sl::RenderCore::Submit(pMeshResource->GetVertexArray(), pShaderResource->GetShaderProgram());
    }

    pFramebuffer->Unbind();
}
