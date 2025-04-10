#include "World.h"

#include <format>

namespace sl
{

Entity World::CreateEntity(std::string_view name)
{
    uint32_t index = 0;
    if (auto it = m_names.find(name.data()); it != m_names.end())
    {
        index = ++it->second;
    }
    else
    {
        m_names[name.data()] = 0;
    }

    Entity entity{ m_registry.create() };
    std::string newName = index ? std::format("{} ({})", name.data(), index) : name.data();

    return entity;
}

bool Entity::IsValid() const
{
    return World::m_registry.valid(m_handle);
}

void Entity::Destroy()
{
    // Destroy this entity and all its components
    World::m_registry.destroy(m_handle);
    m_handle = entt::null;
}

} // namespace sl
