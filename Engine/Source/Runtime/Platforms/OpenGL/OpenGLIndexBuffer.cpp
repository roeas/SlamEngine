#include "OpenGLIndexBuffer.h"

#include <glad/gl.h>

namespace sl
{

OpenGLIndexBuffer::OpenGLIndexBuffer(std::span<const uint32_t> indices) :
    m_handle(0), m_count((uint32_t)indices.size())
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, indices.size_bytes(), indices.data(), 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

} // namespace sl
