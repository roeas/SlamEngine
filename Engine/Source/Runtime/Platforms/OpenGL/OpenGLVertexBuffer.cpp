#include "OpenGLVertexBuffer.h"

#include <glad/gl.h>

namespace sl
{

OpenGLVertexBuffer::OpenGLVertexBuffer(std::span<const float> vertices) :
    m_handle(0)
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, vertices.size_bytes(), vertices.data(), 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

} // namespace sl
