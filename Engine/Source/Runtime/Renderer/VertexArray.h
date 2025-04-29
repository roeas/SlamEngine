#pragma once

#include "Renderer/IndexBuffer.h"
#include "Renderer/Layout.h"
#include "Renderer/VertexBuffer.h"

#include <memory>

namespace sl
{

class VertexArray
{
public:
    static VertexArray *Create(sl::VertexBuffer *pVertexBuffer, sl::IndexBuffer *pIndexBuffer, const VertexLayout &vertexLayout);

public:
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetIndexCount() const = 0;
};

} // namespace sl
