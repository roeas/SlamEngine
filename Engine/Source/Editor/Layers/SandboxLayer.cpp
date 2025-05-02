#include "SandboxLayer.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Resource/ModelImporter.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    // Texture
    std::unique_ptr<sl::TextureResource> pResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/DebugUV.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(sl::StringHash("DebugUV Texture"), std::move(pResource));

    // Shader
    std::unique_ptr<sl::ShaderResource> pBaseShaderIDResource = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/Base_vert.glsl"), sl::Path::FromeAsset("Shader/Base_frag.glsl"));
    std::unique_ptr<sl::ShaderResource> pEntityIDShaderIDResource = std::make_unique<sl::ShaderResource>(
        sl::Path::FromeAsset("Shader/EntityID_vert.glsl"), sl::Path::FromeAsset("Shader/EntityID_frag.glsl"));
    sl::ResourceManager::AddShaderResource(sl::StringHash("Base Shader"), std::move(pBaseShaderIDResource));
    sl::ResourceManager::AddShaderResource(sl::StringHash("EntityID Shader"), std::move(pEntityIDShaderIDResource));

    // Camera
    sl::World::SetMainCameraTransform({ 0.0f, 0.0f, 4.0f }, { 0.0f, glm::radians(-90.0f), 0.0f });

    // Model
    std::string modelPath{ "D:/Works/Model/slum_house/scene.gltf" };
    if (!sl::Path::Exists(modelPath))
    {
        SL_LOG_DEBUG("Hi there, please change the model path at #SandboxLayer.cpp Line 30 if you want to render something.");
    }
    else
    {
        sl::ModelImporter importer{ std::move(modelPath) };
        importer.Import();
    }
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
