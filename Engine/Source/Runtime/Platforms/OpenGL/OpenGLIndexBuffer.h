#pragma once

#include "Renderer/IndexBuffer.h"

namespace sl
{

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(std::span<const uint32_t> indices);
    ~OpenGLIndexBuffer() override;

    uint32_t GetHandle() const override { return m_handle; }
    uint32_t GetCount() const override { return m_count; }

private:
    uint32_t m_count;
    uint32_t m_handle;
};

} // namespace sl
