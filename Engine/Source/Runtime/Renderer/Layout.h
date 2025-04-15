#pragma once

#include "Core/Define.h"

#include <initializer_list>
#include <map>
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

struct UniformBufferLayoutElement
{
    uint32_t m_offset;
    uint32_t m_size;
};

class UniformBufferLayout
{
public:
    void AddElement(std::string_view name, const UniformBufferLayoutElement &element);
    std::optional<UniformBufferLayoutElement> GetElement(std::string_view name) const;

    void SetSize(uint32_t size) { m_size = size; }
    uint32_t GetSize() const { return m_size; }

    auto begin() { return m_elements.begin(); }
    auto end() { return m_elements.end(); }
    auto cbegin() const { return m_elements.cbegin(); }
    auto cend() const { return m_elements.cend(); }

private:
    uint32_t m_size = 0;
    std::map<std::string, UniformBufferLayoutElement> m_elements;
};

} // namespace sl
