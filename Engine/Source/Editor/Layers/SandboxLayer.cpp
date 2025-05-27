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

constexpr sl::StringHashType StandardShaderID = sl::StringHash("Standard Shader");
constexpr sl::StringHashType SkyboxShaderID = sl::StringHash("Skybox Shader");
constexpr sl::StringHashType FullScreenShaderID = sl::StringHash("Full Screen Shader");
constexpr sl::StringHashType EntityIDShaderID = sl::StringHash("EntityID Shader");

constexpr sl::StringHashType SkyTextureID = sl::StringHash("Skybox Texture");
constexpr sl::StringHashType FullScreenTextureID = sl::StringHash("Full Screen Texture");
constexpr sl::StringHashType NoResourceTextureID = sl::StringHash("No Resource Texture");
constexpr sl::StringHashType DebugUVTextureID = sl::StringHash("Debug UV Texture");

} // namespace

SandboxLayer::SandboxLayer()
{
    // Cube mesh
    std::vector<float> cubeVertices
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
    std::vector<uint32_t> cubeIndices
    {
        0, 2, 1, 0, 3, 2,
        5, 7, 4, 5, 6, 7,
        3, 6, 2, 3, 7, 6,
        4, 1, 5, 4, 0, 1,
        4, 3, 0, 4, 7, 3,
        1, 6, 5, 1, 2, 6,
    };
    std::vector<sl::VertexLayoutElement> cubeElements
    {
        { "Position", 3, sl::AttribType::Float, false }
    };
    sl::VertexLayout cubeLayout{ std::move(cubeElements) };
    auto pCubeMesh = std::make_unique<sl::MeshResource>(std::move(cubeVertices), std::move(cubeIndices), std::move(cubeLayout));
    pCubeMesh->SetName("Cube");
    sl::ResourceManager::AddMeshResource(SkyboxMeshID, std::move(pCubeMesh));

    // Quad mesh
    std::vector<float> QuadVertices
    {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,
    };
    std::vector<uint32_t> QuadIndices
    {
        0, 1, 2,
        0, 2, 3,
    };
    std::vector<sl::VertexLayoutElement> QuadElements
    {
        { "Position", 2, sl::AttribType::Float, false },
        { "UV", 2, sl::AttribType::Float, false },
    };
    sl::VertexLayout QuadLayout{ std::move(QuadElements) };
    auto pQuadMesh = std::make_unique<sl::MeshResource>(std::move(QuadVertices), std::move(QuadIndices), std::move(QuadLayout));
    pQuadMesh->SetName("FullScreenQuad");
    sl::ResourceManager::AddMeshResource(FullScreenMeshID, std::move(pQuadMesh));

    // Shaders
    std::unique_ptr<sl::ShaderResource> pStandardShader = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/Standard_vert.glsl"), sl::Path::FromeAsset("Shader/Standard_frag.glsl"));
    std::unique_ptr<sl::ShaderResource> pSkyboxShader = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/Skybox_vert.glsl"), sl::Path::FromeAsset("Shader/Skybox_frag.glsl"));
    std::unique_ptr<sl::ShaderResource> pFullScreenShader = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/FullScreen_vert.glsl"), sl::Path::FromeAsset("Shader/FullScreen_frag.glsl"));
    std::unique_ptr<sl::ShaderResource> pEntityIDShader = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/EntityID_vert.glsl"), sl::Path::FromeAsset("Shader/EntityID_frag.glsl"));
    sl::ResourceManager::AddShaderResource(StandardShaderID, std::move(pStandardShader));
    sl::ResourceManager::AddShaderResource(SkyboxShaderID, std::move(pSkyboxShader));
    sl::ResourceManager::AddShaderResource(FullScreenShaderID, std::move(pFullScreenShader));
    sl::ResourceManager::AddShaderResource(EntityIDShaderID, std::move(pEntityIDShader));

    // Textures
    std::unique_ptr<sl::TextureResource> pSkyboxTexture = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/Sky.ktx"), true, SL_SAMPLER_CLAMP | SL_SAMPLER_LINEAR);
    std::unique_ptr<sl::TextureResource> pNoResourceTexture = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/NoResource.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    std::unique_ptr<sl::TextureResource> pDebugUVTexture = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/DebugUV.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(SkyTextureID, std::move(pSkyboxTexture));
    sl::ResourceManager::AddTextureResource(NoResourceTextureID, std::move(pNoResourceTexture));
    sl::ResourceManager::AddTextureResource(DebugUVTextureID, std::move(pDebugUVTexture));

    // Basic entities
    auto &sky = sl::World::CreateEntity("Sky").AddComponent<sl::SkyComponent>();
    sky.m_meshResourceID = SkyboxMeshID;
    sky.m_shaderResourceID = SkyboxShaderID;
    sky.m_textureResourceID = SkyTextureID;

    auto &postProcessing = sl::World::CreateEntity("Post Processing").AddComponent<sl::PostProcessingComponent>();
    postProcessing.m_meshResourceID = FullScreenMeshID;
    postProcessing.m_shaderResourceID = FullScreenShaderID;

    // Model
    std::string modelPath{ "D:/Works/Model/slum_house/scene.gltf" };
    sl::ModelImporter importer{ std::move(modelPath) };
    importer.SetGlobalScale(1.0f);
    importer.Import();

    // Camera
    sl::World::SetMainCameraTransform({ 4.5f, 6.2f, 12.0f }, { glm::radians(-23.8f), glm::radians(22.8f), 0.0f });
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
