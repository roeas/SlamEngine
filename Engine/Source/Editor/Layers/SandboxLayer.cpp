#include "SandboxLayer.h"

#include "Core/Path.h"
#include "Renderer/RenderCore.h"
#include "Resource/ResourceManager.h"
#include "Scene/World.h"

#include <glm/glm.hpp>
#include <stb/stb_image.h>

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    static float vertices[4 * 5] =
    {
        // Position      // uv
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    };
    static uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

    static std::string vs =
    R"(
        #version 460 core

        layout(std140, binding = 0) uniform UBCamera
        {
            vec4 ub_cameraPos;
            mat4 ub_viewProjection;
        };
        vec3 GetCameraPos()
        {
            return ub_cameraPos.xyz;
        }
        mat4 GetViewProjectionMat()
        {
            return ub_viewProjection;
        }

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec2 a_uv0;

        layout(location = 0) out vec2 v_uv0;

        layout(location = 0) uniform mat4 u_model;

        void main()
        {
            v_uv0 = a_uv0;
            gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
        }
    )";
    static std::string fs =
    R"(
        #version 460 core

        layout(location = 0) in vec2 v_uv0;

        layout(location = 0) out vec4 color;

        layout(binding = 0) uniform sampler2D debugTexture;

        void main()
        {
            color = texture(debugTexture, v_uv0);
        }
    )";

    static std::string IDvs =
    R"(
        #version 460 core
        
        layout(std140, binding = 0) uniform UBCamera
        {
            vec4 ub_cameraPos;
            mat4 ub_viewProjection;
        };
        vec3 GetCameraPos()
        {
            return ub_cameraPos.xyz;
        }
        mat4 GetViewProjectionMat()
        {
            return ub_viewProjection;
        }
        
        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec2 a_uv0;
        
        layout(location = 0) uniform mat4 u_model;
        
        void main()
        {
            gl_Position = GetViewProjectionMat() * u_model * vec4(a_position, 1.0);
        }
    )";

    static std::string IDfs =
    R"(
        #version 460 core
        
        layout(location = 0) out int o_entityID;
        
        layout(location = 1) uniform int u_entityID;
        
        void main()
        {
            o_entityID = u_entityID;
        }
    )";

    auto *pVertexBUffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
    auto *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));
    sl::VertexLayout layout
    {
        { 3, sl::AttribType::Float, false, "Position" },
        { 2, sl::AttribType::Float, false, "UV" },
    };

    sl::VertexArray *pVertexArray = sl::VertexArray::Create(pVertexBUffer, pIndexBuffer, std::move(layout));
    sl::Shader *pShader = sl::Shader::Create(vs, fs);
    sl::Shader *pIDShader = sl::Shader::Create(IDvs, IDfs);

    constexpr sl::StringHashType DebugUVTextureID = sl::StringHash("DebugUV.png");
    std::unique_ptr<sl::TextureResource> pResource = std::make_unique<sl::TextureResource>(
        sl::Path::FromeAsset("Texture/DebugUV.png"), true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR);
    sl::ResourceManager::AddTextureResource(DebugUVTextureID, std::move(pResource));

    auto squareEntity = sl::World::CreateEntity("Square");
    squareEntity.AddComponent<sl::RenderingComponent>(pVertexArray, pShader, pIDShader, DebugUVTextureID);

    sl::World::SetMainCameraTransform({ 0.0f, 0.0f, 4.0f }, { 0.0f, glm::radians(-90.0f), 0.0f });
}

SandboxLayer::~SandboxLayer()
{

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
