#pragma once

#include <cstdint>

namespace sl
{

class IndexBuffer
{
public:
    static IndexBuffer *Create(const uint32_t* pIndicies, size_t size);

public:
    virtual ~IndexBuffer() = default;

    virtual uint32_t GetHandle() const = 0;
    virtual uint32_t GetCount() const = 0;
};

} // namespace sl
