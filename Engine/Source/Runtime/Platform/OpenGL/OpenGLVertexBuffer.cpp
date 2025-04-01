#include "OpenGLVertexBuffer.h"

#include <glad/gl.h>

namespace sl
{

OpenGLVertexBuffer::OpenGLVertexBuffer(const float *pVertices, size_t size)
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, pVertices, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

} // namespace sl
