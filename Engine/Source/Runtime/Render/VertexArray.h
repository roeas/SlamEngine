#pragma once

#include "Render/IndexBuffer.h"
#include "Render/Layout.h"
#include "Render/VertexBuffer.h"

namespace sl
{

class VertexArray
{
public:
    static VertexArray *Create(VertexBuffer *pVertexBuffer, IndexBuffer *pIndexBuffer, VertexLayout vertexLayout);

public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetIndexCount() const = 0;
};

} // namespace sl
