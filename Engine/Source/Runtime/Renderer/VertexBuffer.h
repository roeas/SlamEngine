#pragma once

#include <cstdint>
#include <span>

namespace sl
{

class VertexBuffer
{
public:
    static VertexBuffer *Create(std::span<const float> vertices);

public:
    virtual ~VertexBuffer() = default;

    virtual uint32_t GetHandle() const = 0;
};

} // namespace sl
