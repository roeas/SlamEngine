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

    void Bind() const override;
    void Unbind() const override;

private:
    uint32_t m_handle = 0;
};

} // namespace sl
