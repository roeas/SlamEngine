#include "SandboxLayer.h"

#include "Render/Shader.h"
#include "Render/VertexArray.h"

#include <glad/include/glad/gl.h>

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    static float vertices[3 * 6] =
    {
         // position        // color
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    static uint32_t indices[3] = { 0, 1, 2 };

    static std::string vs =
    R"(
        #version 460 core

        layout(location = 0) in vec3 a_position;
        layout(location = 1) in vec3 a_color;

        layout(location = 0) out vec3 v_color;

        void main()
        {
            v_color = a_color;
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
        { 3, sl::AttribType::Float, false, "Color" },
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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_pVertexArray->Bind();
    m_pShader->Bind();

    glDrawElements(GL_TRIANGLES, m_pVertexArray->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    m_pVertexArray->Unbind();
    m_pShader->Unbind();
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
