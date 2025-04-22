#pragma once

#include "Renderer/VertexArray.h"

#include <memory>

namespace sl
{

class OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray(std::unique_ptr<sl::VertexBuffer> pVertexBuffer,
        std::unique_ptr<sl::IndexBuffer> pIndexBuffer, VertexLayout vertexLayout);
    virtual ~OpenGLVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t GetIndexCount() const override;

private:
    std::unique_ptr<VertexBuffer> m_pVertexBuffer;
    std::unique_ptr<IndexBuffer> m_pIndexBuffer;
    VertexLayout m_vertexLayout;
    uint32_t m_handle;
};

} // namespace sl
