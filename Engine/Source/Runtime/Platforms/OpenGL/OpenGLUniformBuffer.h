#pragma once

#include "Renderer/Layout.h"
#include "Renderer/UniformBuffer.h"

namespace sl
{

class OpenGLUniformBuffer : public UniformBuffer
{
public:
    OpenGLUniformBuffer(uint32_t bindingPoint, UniformBufferLayout layout);
    ~OpenGLUniformBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    void Upload(StringHashType name, const void *pData, uint32_t size = 0) const override;

private:
    UniformBufferLayout m_layout;
    uint32_t m_handle;
};

} // namespace sl
