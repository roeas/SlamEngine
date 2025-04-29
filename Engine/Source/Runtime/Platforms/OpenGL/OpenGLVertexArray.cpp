#include "OpenGLVertexArray.h"

#include "Platforms/OpenGL/OpenGLDefines.h"

#include <glad/gl.h>

namespace sl
{

OpenGLVertexArray::OpenGLVertexArray(std::unique_ptr<sl::VertexBuffer> pVertexBuffer,
    std::unique_ptr<sl::IndexBuffer> pIndexBuffer, const VertexLayout &vertexLayout) :
    m_pVertexBuffer(std::move(pVertexBuffer)), m_pIndexBuffer(std::move(pIndexBuffer)),
    m_handle(0)
{
    glCreateVertexArrays(1, &m_handle);
    glVertexArrayVertexBuffer(m_handle, 0, m_pVertexBuffer->GetHandle(), 0, vertexLayout.GetStride());
    glVertexArrayElementBuffer(m_handle, m_pIndexBuffer->GetHandle());

    uint32_t location = 0;
    for (auto &element : vertexLayout)
    {
        glVertexArrayAttribFormat(
            m_handle,
            location,
            element.m_count,
            GLAttribType[(size_t)element.m_type],
            element.m_normalize ? GL_TRUE : GL_FALSE,
            element.m_offset);
        glVertexArrayAttribBinding(m_handle, location, 0);
        glEnableVertexArrayAttrib(m_handle, location);

        ++location;
    }
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_handle);
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_handle);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

uint32_t OpenGLVertexArray::GetIndexCount() const
{
    return m_pIndexBuffer->GetCount();
}

} // namespace sl
