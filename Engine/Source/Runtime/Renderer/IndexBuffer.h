#pragma once

#include <cstdint>
#include <span>

namespace sl
{

class IndexBuffer
{
public:
    static IndexBuffer *Create(std::span<const uint32_t> indices);

public:
    virtual ~IndexBuffer() = default;

    virtual uint32_t GetHandle() const = 0;
    virtual uint32_t GetCount() const = 0;
};

} // namespace sl
