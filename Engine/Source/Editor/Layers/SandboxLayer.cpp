#include "SandboxLayer.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Resource/ModelImporter.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    // Camera
    sl::World::SetMainCameraTransform({ 0.0f, 0.0f, 4.0f }, { 0.0f, glm::radians(-90.0f), 0.0f });

    // Model
    std::string modelPath{ "D:/Works/Model/slum_house/scene.gltf" };
    if (!sl::Path::Exists(modelPath))
    {
        SL_LOG_DEBUG("Hi there, please change the model path at #SandboxLayer.cpp line 22 if you want to render something.");
        SL_LOG_DEBUG("To be honest, only gltf material is well supported right now.");
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
