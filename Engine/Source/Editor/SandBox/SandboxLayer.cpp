#include "SandboxLayer.h"

#include "Render/IndexBuffer.h"
#include "Render/Layout.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"

#include <glad/include/glad/gl.h>

// Just some temporary codes here

SandboxLayer::SandboxLayer()
{
    static uint32_t indices[3] = { 0, 1, 2 };
    static float vertices[3 * 6] =
    {
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

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

    m_pIndexBuffer.reset(sl::IndexBuffer::Create(indices, sizeof(indices)));
    m_pVertexBUffer.reset(sl::VertexBuffer::Create(vertices, sizeof(vertices)));
    sl::VertexLayout layout
    {
        { 3, sl::AttribType::Float, false, "Position" },
        { 3, sl::AttribType::Float, false, "Color" },
    };

    // m_pVertexArrayBuffer.reset(sl::VertexArrayBuffer::Create(std::move(pIndexBuffer), std::move(pVertexBUffer), std::move(layout)));

    glCreateVertexArrays(1, &m_vao);
    glVertexArrayVertexBuffer(m_vao, 0, m_pVertexBUffer->GetHandle(), 0, 6 * sizeof(float));

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glEnableVertexArrayAttrib(m_vao, 0);

    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glEnableVertexArrayAttrib(m_vao, 1);

    glVertexArrayElementBuffer(m_vao, m_pIndexBuffer->GetHandle());

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

    glBindVertexArray(m_vao);
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
