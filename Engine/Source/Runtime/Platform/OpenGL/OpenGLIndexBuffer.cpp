#include "OpenGLIndexBuffer.h"

#include <glad/gl.h>

namespace sl
{

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t *pIndices, size_t size) :
    m_handle(0), m_count((uint32_t)size / sizeof(uint32_t))
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, size, pIndices, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

} // namespace sl
