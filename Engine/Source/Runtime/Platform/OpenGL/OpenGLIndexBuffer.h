#pragma once

#include "Render/IndexBuffer.h"

namespace sl
{

class OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(const uint32_t *pIndices, size_t size);
    ~OpenGLIndexBuffer() override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t GetCount() const override { return m_count; }

private:
    uint32_t m_handle = 0;
    uint32_t m_count = 0;
};

} // namespace sl
