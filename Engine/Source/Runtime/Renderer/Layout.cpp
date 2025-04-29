#include "Layout.h"

#include "Core/Log.h"

#include <magic_enum/magic_enum.hpp>

namespace sl
{

namespace
{

constexpr uint32_t AttribTypeSize[magic_enum::enum_count<AttribType>()] =
{
    1, // AttribType::Int8
    1, // AttribType::Uint8
    2, // AttribType::Int16
    2, // AttribType::Uint16
    4, // AttribType::Int32
    4, // AttribType::Uint32
    2, // AttribType::Half
    4, // AttribType::Float
    8, // AttribType::Double
};

} // namespace

VertexLayoutElement::VertexLayoutElement(const char *pName, uint32_t count, AttribType type, bool normalize) :
    m_name(pName), m_count(count), m_offset(0), m_size(count * AttribTypeSize[(size_t)type]),
    m_type(type), m_normalize(normalize)
{
    
}

VertexLayout::VertexLayout(std::initializer_list<VertexLayoutElement> elements) :
    m_elements(std::move(elements)), m_stride(0)
{
    uint32_t offset = 0;
    for (auto &element : m_elements)
    {
        element.m_offset = offset;
        offset += element.m_size;
        m_stride += element.m_size;
    }
}

void UniformBufferLayout::AddElement(StringHashType name, const UniformBufferLayoutElement &element)
{
    if (!m_elements.contains(name))
    {
        m_elements.emplace(name, element);
    }
    else
    {
        SL_LOG_ERROR("Uniform buffer element already exists!");
    }
}

std::optional<UniformBufferLayoutElement> UniformBufferLayout::GetElement(StringHashType name) const
{
    if (auto it = m_elements.find(name); it != m_elements.end())
    {
        return it->second;
    }
    else
    {
        return std::nullopt;
    }
}

} // namespace sl
