#pragma once

#include "Core/Defines.h"

#include <string>
#include <vector>

namespace sl
{

struct VertexLayoutElement
{
    VertexLayoutElement(uint32_t count, AttribType type, bool normalize, std::string_view name);

    std::string m_name;
    uint32_t m_count;
    uint32_t m_size;
    uint32_t m_offset;
    AttribType m_type;
    bool m_normalize;
};

class VertexLayout final
{
public:
    VertexLayout() = default;
    VertexLayout(std::initializer_list<VertexLayoutElement> elements);

    uint32_t GetStride() const { return m_stride; }
    uint32_t GetElementCount() const { return (uint32_t)m_elements.size(); }

    auto begin() { return m_elements.begin(); }
    auto end() { return m_elements.end(); }
    auto begin() const { return m_elements.cbegin(); }
    auto end() const { return m_elements.end(); }

private:
    std::vector<VertexLayoutElement> m_elements;
    uint32_t m_stride;
};

} // namespace sl
