#include "SandboxLayer.h"

#include "Core/Path.h"
#include "Renderer/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"
#include "Utils/ProfilerCPU.h"

#include <glm/glm.hpp>
#include <stb/stb_image.h>

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    SL_PROFILE;

    std::vector<float> vertices
    {
        // Position         // uv
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };
    std::vector<uint32_t> indices{ 0, 1, 2, 2, 3, 0 };
    std::vector<sl::VertexLayoutElement> elements
    {
        { "Position", 3, sl::AttribType::Float, false },
        { "UV", 2, sl::AttribType::Float, false },
    };

    // Hash
    constexpr sl::StringHashType SquareMeshID = sl::StringHash("Square Mesh");
    constexpr sl::StringHashType DebugUVTextureID = sl::StringHash("DebugUV Texture");
    constexpr sl::StringHashType BaseShaderID = sl::StringHash("Base Shader");
    constexpr sl::StringHashType EntityIDShaderID = sl::StringHash("EntityID Shader");

    // Mesh
    std::unique_ptr<sl::MeshResource> pSquareMesh = std::make_unique<sl::MeshResource>(
        std::move(vertices), std::move(indices), sl::VertexLayout{ std::move(elements) });
    sl::ResourceManager::AddMeshResource(SquareMeshID, std::move(pSquareMesh));

    // Texture
    std::unique_ptr<sl::TextureResource> pResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/DebugUV.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(DebugUVTextureID, std::move(pResource));

    // Shader
    std::unique_ptr<sl::ShaderResource> pBaseShaderIDResource = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/Base_vert.glsl"), sl::Path::FromeAsset("Shader/Base_frag.glsl"));
    std::unique_ptr<sl::ShaderResource> pEntityIDShaderIDResource = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/EntityID_vert.glsl"), sl::Path::FromeAsset("Shader/EntityID_frag.glsl"));
    sl::ResourceManager::AddShaderResource(BaseShaderID, std::move(pBaseShaderIDResource));
    sl::ResourceManager::AddShaderResource(EntityIDShaderID, std::move(pEntityIDShaderIDResource));

    auto squareEntity = sl::World::CreateEntity("Square");
    squareEntity.AddComponent<sl::RenderingComponent>(SquareMeshID, DebugUVTextureID, BaseShaderID, EntityIDShaderID);

    sl::World::SetMainCameraTransform({ 0.0f, 0.0f, 4.0f }, { 0.0f, glm::radians(-90.0f), 0.0f });
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
