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

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;
};

} // namespace sl
