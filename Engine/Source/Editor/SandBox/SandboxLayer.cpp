#include "SandboxLayer.h"

#include "Render/IndexBuffer.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"

#include <glad/include/glad/gl.h>

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    static float vertices[3 * 3] =
    {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
    };
    static uint32_t indices[3] = { 0, 1, 2 };

    static std::string vs =
    R"(
        #version 460 core

        layout(location = 0) in vec3 a_position;

        void main()
        {
            gl_Position = vec4(a_position, 1.0);
        }
    )";
    static std::string fs =
    R"(
        #version 460 core

        layout(location = 0) out vec4 color;

        void main()
        {
            color = vec4(0.5, 0.7, 0.9, 1.0);
        }
    )";

    glGenVertexArrays(1, &m_va);
    glBindVertexArray(m_va);

    m_pVertexBUffer.reset(sl::VertexBuffer::Create(vertices, sizeof(vertices)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    m_pIndexBuffer.reset(sl::IndexBuffer::Create(indices, sizeof(indices)));

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

    glBindVertexArray(m_va);
    m_pShader->Bind();

    glDrawElements(GL_TRIANGLES, m_pIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
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
