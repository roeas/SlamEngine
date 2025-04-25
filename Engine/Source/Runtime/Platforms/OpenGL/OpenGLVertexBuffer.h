#pragma once

#include "Renderer/VertexBuffer.h"

#include <cstdint>

namespace sl
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(std::span<const float> vertices);
    ~OpenGLVertexBuffer() override;

    uint32_t GetHandle() const override { return m_handle; }

private:
    uint32_t m_handle;
};

} // namespace sl
