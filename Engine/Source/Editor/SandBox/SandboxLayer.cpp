#include "SandboxLayer.h"

#include "Core/Path.hpp"
#include "Renderer/RenderCore.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include <stb/stb_image.h>

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    static float vertices[4 * 5] =
    {
        // position         // uv
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
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
        layout(location = 1) in vec2 a_uv;

        layout(location = 0) out vec2 v_uv;

        void main()
        {
            v_uv = a_uv;
            gl_Position = vec4(a_position, 1.0);
        }
    )";
    static std::string fs =
    R"(
        #version 460 core

        layout(location = 0) in vec2 v_uv;

        layout(location = 0) out vec4 color;

        layout(binding = 0) uniform sampler2D debugTexture;

        void main()
        {
            color = texture(debugTexture, v_uv);
        }
    )";

    auto *pVertexBUffer = sl::VertexBuffer::Create(vertices, sizeof(vertices));
    auto *pIndexBuffer = sl::IndexBuffer::Create(indices, sizeof(indices));
    sl::VertexLayout layout
    {
        { 3, sl::AttribType::Float, false, "Position" },
        { 2, sl::AttribType::Float, false, "UV" },
    };

    m_pVertexArray.reset(sl::VertexArray::Create(pVertexBUffer, pIndexBuffer, std::move(layout)));
    m_pShader.reset(sl::Shader::Create(vs, fs));

    int width, height, channel;
    stbi_set_flip_vertically_on_load(1);
    auto *pData = stbi_load(sl::Path::FromeAsset("Texture/DebugUV.png").data(), &width, &height, &channel, 3);
    m_pTexture.reset(sl::Texture2D::Create(width, height, sl::TextureFormat::RGB8, true, SL_SAMPLER_REPEAT | SL_SAMPLER_LINEAR, pData));
    stbi_image_free(pData);
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
    sl::RenderCore::GetMainFramebuffer()->Bind();

    sl::RenderCore::SetClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    sl::RenderCore::Clear();

    m_pTexture->Bind(0);
    sl::RenderCore::Submit(m_pVertexArray.get(), m_pShader.get());

    sl::RenderCore::GetMainFramebuffer()->Unbind();
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
