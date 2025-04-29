#pragma once

#include "Core/Log.h"
#include "Scene/CameraComponent.h"
#include "Scene/RenderingComponent.h"
#include "Scene/TagComponent.h"
#include "Scene/TransformComponent.h"
#include "Utils/NameOf.hpp"

#include <entt/entt.hpp>

namespace sl
{

class World final
{
    friend class Entity;

public:
    World() = delete;
    World(const World &) = delete;
    World &operator=(const World &) = delete;
    World(World &&) = delete;
    World &operator=(World &&) = delete;
    ~World() = delete;

    static entt::registry &GetRegistry() { return registry; }
    static Entity CreateEntity(std::string_view name = "New Entity");

    static Entity GetMainCameraEntity();
    static CameraComponent &GetMainCameraComponent();
    static TransformComponent &GetMainCameraTransformComponent();
    static void SetMainCameraTransform(glm::vec3 position, glm::vec3 rotation);

    template<typename T>
    static void Clear() { registry.clear<T>(); }
    static void Clear() { registry.clear(); }

private:
    inline static entt::registry registry;
};

// Basically, #Entity is just a tool class that allows us to use ECS more intuitively
class Entity final
{
public:
    Entity() = default;
    Entity(entt::entity entity) : m_handle(entity) {}
    Entity(uint32_t entity) : m_handle((entt::entity)entity) {}

    entt::entity GetHandle() const { return m_handle; }
    void ResetHandle() { m_handle = entt::null; }
    bool IsValid() const;
    void Destroy();

    // Returns reference
    template<typename T, typename... Args>
    decltype(auto) AddComponent(Args &&...args)
    {
        SL_ASSERT(!HasAllComponentsOf<T>(), "Entity already holds {} component!", nameof::nameof_type<T>());
        return World::registry.emplace<T>(m_handle, std::forward<Args>(args)...);
    }

    // Returns reference
    template<typename T, typename... Args>
    decltype(auto) ReplaceComponent(Args &&...args)
    {
        SL_ASSERT(HasAllComponentsOf<T>(), "Entity does not hold {} component!", nameof::nameof_type<T>());
        return World::registry.replace<T>(m_handle, std::forward<Args>(args)...);
    }

    // Returns reference
    template<typename T, typename... Args>
    decltype(auto) AddOrReplaceComponent(Args &&...args)
    {
        return World::registry.emplace_or_replace<T>(m_handle, std::forward<Args>(args)...);
    }

    // Returns tuple of references
    template<typename... T>
    decltype(auto) GetComponents()
    {
        SL_ASSERT(HasAllComponentsOf<T...>(), "Entity does not hold {} component!", nameof::nameof_types<T...>());
        return World::registry.get<T...>(m_handle);
    }

    // Returns tuple of pointers
    template<typename... T>
    auto TryGetComponents()
    {
        return World::registry.try_get<T...>(m_handle);
    }

    template<typename... T>
    bool HasAllComponentsOf()
    {
        return World::registry.all_of<T...>(m_handle);
    }

    template<typename... T>
    bool HasAnyComponentsOf()
    {
        return World::registry.any_of<T...>(m_handle);
    }

    bool HasAnyComponent()
    {
        return !World::registry.orphan(m_handle);
    }

    template<typename T>
    auto RemoveComponent()
    {
        return World::registry.remove<T>(m_handle);
    }

    bool operator==(const Entity &other) const { return m_handle == other.m_handle; }
    bool operator!=(const Entity &other) const { return !(operator==(other)); }

private:
    entt::entity m_handle = entt::null;
};

} // namespace sl

inline bool operator==(const sl::Entity &entity, const entt::entity &other) { return entity.GetHandle() == other; }
inline bool operator!=(const sl::Entity &entity, const entt::entity &other) { return !operator==(entity, other); }
inline bool operator==(const entt::entity &other, const sl::Entity &entity) { return other == entity.GetHandle(); }
inline bool operator!=(const entt::entity &other, const sl::Entity &entity) { return !operator==(other, entity); }

inline bool operator==(const sl::Entity &entity, const uint32_t &other) { return (uint32_t)entity.GetHandle() == other; }
inline bool operator!=(const sl::Entity &entity, const uint32_t &other) { return !operator==(entity, other); }
inline bool operator==(const uint32_t &other, const sl::Entity &entity) { return other == (uint32_t)entity.GetHandle(); }
inline bool operator!=(const uint32_t &other, const sl::Entity &entity) { return !operator==(other, entity); }
