#pragma once

#include "Renderer/IndexBuffer.h"

namespace sl
{

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(const uint32_t *pIndices, size_t size);
    ~OpenGLIndexBuffer() override;

    uint32_t GetHandle() const override { return m_handle; }
    uint32_t GetCount() const override { return m_count; }

private:
    uint32_t m_handle;
    uint32_t m_count;
};

} // namespace sl
