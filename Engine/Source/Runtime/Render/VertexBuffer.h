#pragma once

namespace sl
{

class VertexBuffer
{
public:
    static VertexBuffer *Create(const float *pVertices, size_t size);

public:
    virtual ~VertexBuffer() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};

} // namespace sl
