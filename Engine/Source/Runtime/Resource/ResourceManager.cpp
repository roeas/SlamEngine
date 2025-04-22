#include "ResourceManager.h"

#include "Core/Log.h"

#include <nameof/nameof.hpp>

namespace sl
{

void ResourceManager::Update()
{
    for (auto &[_, pResource] : m_pTextureResources)
    {
        pResource->Update();
    }
}

void ResourceManager::AddTextureResource(StringHashType key, std::unique_ptr<TextureResource> pResource)
{
    AddResource(key, std::move(pResource));
}

TextureResource *ResourceManager::GetTextureResource(StringHashType key)
{
    return GetResource<TextureResource>(key);
}

template<class T>
void ResourceManager::AddResource(StringHashType key, std::unique_ptr<T> pResource)
{
    if constexpr (std::same_as<T, TextureResource>)
    {
        if (!m_pTextureResources.contains(key))
        {
            m_pTextureResources.emplace(key, std::move(pResource));
            return;
        }
    }
    else
    {
        SL_LOG_ERROR("Invalid resource type {}", nameof::nameof_type<T>());
        return;
    }

    SL_LOG_WARN("Resource {} already exists!", key);
}

template<class T>
T *ResourceManager::GetResource(StringHashType key)
{
    if constexpr (std::same_as<T, TextureResource>)
    {
        if (auto it = m_pTextureResources.find(key); it != m_pTextureResources.end())
        {
            return it->second.get();
        }
    }
    else
    {
        SL_LOG_ERROR("Invalid resource type {}", nameof::nameof_type<T>());
    }

    return nullptr;
}

} // namespace sl
