#include "SandboxLayer.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Resource/ModelImporter.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"

// Just some temporary codes here

namespace
{

constexpr sl::StringHashType SkyboxMeshID = sl::StringHash("Skybox Mesh");
constexpr sl::StringHashType FullScreenMeshID = sl::StringHash("Full Screen Mesh");

constexpr sl::StringHashType BaseShaderID = sl::StringHash("Base Shader");
constexpr sl::StringHashType SkyboxShaderID = sl::StringHash("Skybox Shader");
constexpr sl::StringHashType FullScreenShaderID = sl::StringHash("Full Screen Shader");
constexpr sl::StringHashType EntityIDShaderID = sl::StringHash("EntityID Shader");

constexpr sl::StringHashType RadianceTextureID = sl::StringHash("Radiance Texture");
constexpr sl::StringHashType IrradianceTextureID = sl::StringHash("Irradiance Texture");
constexpr sl::StringHashType SkyTextureID = sl::StringHash("Skybox Texture");
constexpr sl::StringHashType FullScreenTextureID = sl::StringHash("Full Screen Texture");
constexpr sl::StringHashType NoResourceTextureID = sl::StringHash("No Resource Texture");
constexpr sl::StringHashType DebugUVTextureID = sl::StringHash("Debug UV Texture");

} // namespace

SandboxLayer::SandboxLayer()
{
    // Meshes
    {
        std::vector<float> vertices
        {
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
        };
        std::vector<uint32_t> indices
        {
            0, 2, 1, 0, 3, 2,
            5, 7, 4, 5, 6, 7,
            3, 6, 2, 3, 7, 6,
            4, 1, 5, 4, 0, 1,
            4, 3, 0, 4, 7, 3,
            1, 6, 5, 1, 2, 6,
        };
        std::vector<sl::VertexLayoutElement> elements
        {
            { "Position", 3, sl::AttribType::Float, false }
        };
        sl::VertexLayout layout{ std::move(elements) };
        auto pMeshResource = std::make_unique<sl::MeshResource>(std::move(vertices), std::move(indices), std::move(layout));
        pMeshResource->SetName("Cube");
        sl::ResourceManager::AddMeshResource(SkyboxMeshID, std::move(pMeshResource));
    }

    {
        std::vector<float> vertices
        {
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,
        };
        std::vector<uint32_t> indices
        {
            0, 1, 2,
            0, 2, 3,
        };
        std::vector<sl::VertexLayoutElement> elements
        {
            { "Position", 2, sl::AttribType::Float, false },
            { "UV", 2, sl::AttribType::Float, false },
        };
        sl::VertexLayout layout{ std::move(elements) };
        auto pMeshResource = std::make_unique<sl::MeshResource>(std::move(vertices), std::move(indices), std::move(layout));
        pMeshResource->SetName("FullScreenQuad");
        sl::ResourceManager::AddMeshResource(FullScreenMeshID, std::move(pMeshResource));
    }

    // Shaders
    {
        std::unique_ptr<sl::ShaderResource> pBaseShaderResource = std::make_unique<sl::ShaderResource>(
            sl::Path::FromeAsset("Shader/Base_vert.glsl"), sl::Path::FromeAsset("Shader/Base_frag.glsl"));
        std::unique_ptr<sl::ShaderResource> pSkyboxShaderResource = std::make_unique<sl::ShaderResource>(
            sl::Path::FromeAsset("Shader/Skybox_vert.glsl"), sl::Path::FromeAsset("Shader/Skybox_frag.glsl"));
        std::unique_ptr<sl::ShaderResource> pFullScreenShaderResource = std::make_unique<sl::ShaderResource>(
            sl::Path::FromeAsset("Shader/FullScreen_vert.glsl"), sl::Path::FromeAsset("Shader/FullScreen_frag.glsl"));
        std::unique_ptr<sl::ShaderResource> pEntityIDShaderResource = std::make_unique<sl::ShaderResource>(
            sl::Path::FromeAsset("Shader/EntityID_vert.glsl"), sl::Path::FromeAsset("Shader/EntityID_frag.glsl"));
        sl::ResourceManager::AddShaderResource(BaseShaderID, std::move(pBaseShaderResource));
        sl::ResourceManager::AddShaderResource(SkyboxShaderID, std::move(pSkyboxShaderResource));
        sl::ResourceManager::AddShaderResource(FullScreenShaderID, std::move(pFullScreenShaderResource));
        sl::ResourceManager::AddShaderResource(EntityIDShaderID, std::move(pEntityIDShaderResource));
    }

    // Textures
    {
        std::unique_ptr<sl::TextureResource> pRadianceTextureResource = std::make_unique<sl::TextureResource>(
            sl::Path::FromeAsset("Texture/Rad.ktx"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
        std::unique_ptr<sl::TextureResource> pIrradianceTextureResource = std::make_unique<sl::TextureResource>(
            sl::Path::FromeAsset("Texture/Irr.ktx"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
        std::unique_ptr<sl::TextureResource> pSkyboxTextureResource = std::make_unique<sl::TextureResource>(
            sl::Path::FromeAsset("Texture/Sky.ktx"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
        std::unique_ptr<sl::TextureResource> pNoResourceTextureResource = std::make_unique<sl::TextureResource>(
            sl::Path::FromeAsset("Texture/NoResource.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
        std::unique_ptr<sl::TextureResource> pDebugUVTextureResource = std::make_unique<sl::TextureResource>(
            sl::Path::FromeAsset("Texture/DebugUV.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
        sl::ResourceManager::AddTextureResource(RadianceTextureID, std::move(pRadianceTextureResource));
        sl::ResourceManager::AddTextureResource(IrradianceTextureID, std::move(pIrradianceTextureResource));
        sl::ResourceManager::AddTextureResource(SkyTextureID, std::move(pSkyboxTextureResource));
        sl::ResourceManager::AddTextureResource(NoResourceTextureID, std::move(pNoResourceTextureResource));
        sl::ResourceManager::AddTextureResource(DebugUVTextureID, std::move(pDebugUVTextureResource));
    }

    // Basic entities
    {
        auto &skyComponent = sl::World::CreateEntity("Sky").AddComponent<sl::SkyComponent>();
        skyComponent.m_meshResourceID = SkyboxMeshID;
        skyComponent.m_shaderResourceID = SkyboxShaderID;
        skyComponent.m_textureResourceID = SkyTextureID;

        auto &postProcessingComponent = sl::World::CreateEntity("Post Processing").AddComponent<sl::PostProcessingComponent>();
        postProcessingComponent.m_meshResourceID = FullScreenMeshID;
        postProcessingComponent.m_shaderResourceID = FullScreenShaderID;
    }

    // Model
    std::string modelPath{ "D:/Works/Model/slum_house/scene.gltf" };
    if (!sl::Path::Exists(modelPath))
    {
        SL_LOG_DEBUG("Hi there, please change the model path at {} line {} if you want to render something.",
            sl::Path::NameWithExtension(__FILE__), __LINE__ - 4);
        SL_LOG_DEBUG("To be honest, only gltf material is well supported right now.");
    }
    else
    {
        sl::ModelImporter importer{ std::move(modelPath) };
        importer.Import();
    }

    // Camera
    sl::World::SetMainCameraTransform({ 6.0f, 5.5f, 12.0f }, { glm::radians(-18.5f), glm::radians(-118.0f), 0.0f });
}

void SandboxLayer::OnAttach()
{

}

void SandboxLayer::OnDetach()
{

}

void SandboxLayer::BeginFrame()
{

}

void SandboxLayer::OnUpdate(float deltaTime)
{

}

void SandboxLayer::OnRender()
{

}

void SandboxLayer::EndFrame()
{

}

void SandboxLayer::OnEvent(sl::Event &event)
{

}
