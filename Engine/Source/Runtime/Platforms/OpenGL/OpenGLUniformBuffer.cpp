#include "OpenGLUniformBuffer.h"

#include "Core/Log.h"

#include <glad/gl.h>

namespace sl
{

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t bindingPoint, UniformBufferLayout layout) :
    m_layout(std::move(layout)), m_handle(0)
{
    glGenBuffers(1, &m_handle);

    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    glBufferData(GL_UNIFORM_BUFFER, m_layout.GetSize(), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_handle);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

void OpenGLUniformBuffer::Bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
}

void OpenGLUniformBuffer::Unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBuffer::Upload(std::string_view name, const void *pData, uint32_t size) const
{
    if (const auto &optElement = m_layout.GetElement(name); optElement)
    {
        if (size > optElement->m_size)
        {
            SL_LOG_ERROR("Uniform buffer element {} update size out of range!", name.data());
            return;
        }

        glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
        glBufferSubData(GL_UNIFORM_BUFFER, optElement->m_offset, size > 0 ? size : optElement->m_size, pData);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    else
    {
        SL_LOG_ERROR("Uniform buffer element {} does not exist!", name.data());
    }
}

} // namespace sl
