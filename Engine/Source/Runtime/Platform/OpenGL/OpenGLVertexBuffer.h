#pragma once

#include "Render/VertexBuffer.h"

#include <cstdint>

namespace sl
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(const float *pVertices, size_t size);
    ~OpenGLVertexBuffer() override;

    uint32_t GetHandle() const override { return m_handle; }

private:
    uint32_t m_handle = 0;
};

} // namespace sl
