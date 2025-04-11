#include "World.h"

namespace sl
{

Entity World::CreateEntity(std::string_view name)
{
    Entity entity{ m_registry.create() };
    entity.AddComponent<TagComponent>(name.data());
    entity.AddComponent<TransformComponent>();

    return entity;
}

Entity World::GetMainCameraEntity()
{
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view)
    {
        if (view.get<CameraComponent>(entity).m_isMainCamera)
        {
            return entity;
        }
    }

    return {};
}

CameraComponent &World::GetMainCameraComponent()
{
    return GetMainCameraEntity().GetComponents<CameraComponent>();
}

TransformComponent &World::GetMainCameraTransformComponent()
{
    return GetMainCameraEntity().GetComponents<TransformComponent>();
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
