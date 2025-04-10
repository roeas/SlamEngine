#include "World.h"

#include "Scene/TagComponent.h"
#include "Scene/TransformComponent.h"

#include <format>

namespace sl
{

Entity World::CreateEntity(std::string_view name)
{
    Entity entity{ m_registry.create() };
    entity.AddComponent<TagComponent>(name.data());
    entity.AddComponent<TransformComponent>();

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
