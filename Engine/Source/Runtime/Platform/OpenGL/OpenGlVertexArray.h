#pragma once

#include "Renderer/VertexArray.h"

#include <memory>

namespace sl
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray(VertexBuffer *pVertexBuffer, IndexBuffer *pIndexBuffer, VertexLayout vertexLayout);
    virtual ~OpenGLVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t GetIndexCount() const override;

private:
    VertexLayout m_vertexLayout;
    std::unique_ptr<VertexBuffer> m_pVertexBuffer;
    std::unique_ptr<IndexBuffer> m_pIndexBuffer;
    uint32_t m_handle = 0;
};

} // namespace sl
