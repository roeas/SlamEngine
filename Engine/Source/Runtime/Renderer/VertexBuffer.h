#pragma once

#include <cstdint>

namespace sl
{

class VertexBuffer
{
public:
    static VertexBuffer *Create(const float *pVertices, size_t size);

public:
    virtual ~VertexBuffer() = default;

    virtual uint32_t GetHandle() const = 0;
};

} // namespace sl
