#pragma once

#include "Core/Defines.h"
#include "Utils/Hash.hpp"

#include <map>
#include <string>
#include <vector>

namespace sl
{

struct VertexLayoutElement
{
    VertexLayoutElement(const char *pName, uint32_t count, AttribType type, bool normalize);

    const char *m_pName;
    uint32_t m_count;
    uint32_t m_offset;
    uint32_t m_size;
    AttribType m_type;
    bool m_normalize;
};

class VertexLayout final
{
public:
    VertexLayout(std::vector<VertexLayoutElement> elements);

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

struct UniformBufferLayoutElement
{
    uint32_t m_offset;
    uint32_t m_size;
};

// TODO: Currently we have to manually calculate the std140 layout, optimize it
class UniformBufferLayout
{
public:
    void AddElement(StringHashType name, const UniformBufferLayoutElement &element);
    std::optional<UniformBufferLayoutElement> GetElement(StringHashType name) const;

    void SetSize(uint32_t size) { m_size = size; }
    uint32_t GetSize() const { return m_size; }

    auto begin() { return m_elements.begin(); }
    auto end() { return m_elements.end(); }
    auto cbegin() const { return m_elements.cbegin(); }
    auto cend() const { return m_elements.cend(); }

private:
    std::map<StringHashType, UniformBufferLayoutElement> m_elements;
    uint32_t m_size = 0;
};

} // namespace sl
