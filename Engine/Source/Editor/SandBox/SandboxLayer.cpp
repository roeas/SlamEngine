#include "SandboxLayer.h"

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/RenderCore.h"

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

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec2 a_uv;

        layout(location = 0) out vec3 v_color;

        void main()
        {
            v_color = vec3(a_uv, 0.0f);
            gl_Position = vec4(a_position, 1.0);
        }
    )";
    static std::string fs =
    R"(
        #version 460 core

        layout(location = 0) in vec3 v_color;

        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(v_color, 1.0);
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
    sl::RenderCore::SetClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    sl::RenderCore::Clear();

    sl::RenderCore::Submit(m_pVertexArray.get(), m_pShader.get());
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
