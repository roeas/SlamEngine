#include "SandboxLayer.h"

#include "Render/Shader.h"

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

    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    m_pShader->Bind();
    glBindVertexArray(m_va);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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
