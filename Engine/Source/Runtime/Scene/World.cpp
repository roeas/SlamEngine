#include "World.h"

namespace sl
{

namespace
{

bool EntityNameMatch(std::string_view base, std::string_view str)
{
    if (base == str)
    {
        return true;
    }

    size_t baseSize = base.size();
    size_t strSize = str.size();
    if (strSize < baseSize + 4 ||
        str.find(base) != 0 ||
        str[baseSize] != ' ' ||
        str[baseSize + 1] != '(' ||
        str[strSize - 1] != ')')
    {
        return false;
    }
    for (size_t i = baseSize + 2; i < strSize - 1; ++i)
    {
        if (!std::isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

}

Entity World::CreateEntity(std::string_view name)
{
    uint32_t sameNameCount = 0;
    std::string newName = name.data();
    auto view = m_registry.view<TagComponent>();
    for (auto sameNameEntity : view)
    {
        if (EntityNameMatch(name, view.get<TagComponent>(sameNameEntity).m_name))
        {
            ++sameNameCount;
        }
    }
    if (sameNameCount > 0)
    {
        newName += std::format(" ({})", sameNameCount);
    }

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
