#include "ResourceManager.h"

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

#include <nameof/nameof.hpp>

namespace sl
{

void ResourceManager::Update()
{
    SL_PROFILE;

    for (auto &[_, pResource] : m_pMeshResources)
    {
        pResource->Update();
    }
    for (auto &[_, pResource] : m_pTextureResources)
    {
        pResource->Update();
    }
    for (auto &[_, pResource] : m_pShaderResources)
    {
        pResource->Update();
    }
    for (auto &[_, pResource] : m_pMaterialResources)
    {
        pResource->Update();
    }
}

void ResourceManager::AddMeshResource(StringHashType key, std::unique_ptr<MeshResource> pResource)
{
    AddResource<MeshResource>(key, std::move(pResource));
}

MeshResource *ResourceManager::GetMeshResource(StringHashType key)
{
    return GetResource<MeshResource>(key);
}

void ResourceManager::AddTextureResource(StringHashType key, std::unique_ptr<TextureResource> pResource)
{
    AddResource<TextureResource>(key, std::move(pResource));
}

TextureResource *ResourceManager::GetTextureResource(StringHashType key)
{
    return GetResource<TextureResource>(key);
}

void ResourceManager::AddShaderResource(StringHashType key, std::unique_ptr<ShaderResource> pResource)
{
    AddResource<ShaderResource>(key, std::move(pResource));
}

ShaderResource *ResourceManager::GetShaderResource(StringHashType key)
{
    return GetResource<ShaderResource>(key);
}

void ResourceManager::AddMaterialResource(StringHashType key, std::unique_ptr<MaterialResource> pResource)
{
    AddResource<MaterialResource>(key, std::move(pResource));
}

MaterialResource *ResourceManager::GetMaterialResource(StringHashType key)
{
    return GetResource<MaterialResource>(key);
}

template<class T>
void ResourceManager::AddResource(StringHashType key, std::unique_ptr<T> pResource)
{
    if constexpr (std::same_as<T, MeshResource>)
    {
        if (!m_pMeshResources.contains(key))
        {
            m_pMeshResources.emplace(key, std::move(pResource));
            return;
        }
    }
    else if constexpr (std::same_as<T, TextureResource>)
    {
        if (!m_pTextureResources.contains(key))
        {
            m_pTextureResources.emplace(key, std::move(pResource));
            return;
        }
    }
    else if constexpr (std::same_as<T, ShaderResource>)
    {
        if (!m_pShaderResources.contains(key))
        {
            m_pShaderResources.emplace(key, std::move(pResource));
            return;
        }
    }
    else if constexpr (std::same_as<T, MaterialResource>)
    {
        if (!m_pMaterialResources.contains(key))
        {
            m_pMaterialResources.emplace(key, std::move(pResource));
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
    if constexpr (std::same_as<T, MeshResource>)
    {
        if (auto it = m_pMeshResources.find(key); it != m_pMeshResources.end())
        {
            return it->second.get();
        }
    }
    else if constexpr (std::same_as<T, TextureResource>)
    {
        if (auto it = m_pTextureResources.find(key); it != m_pTextureResources.end())
        {
            return it->second.get();
        }
    }
    else if constexpr (std::same_as<T, ShaderResource>)
    {
        if (auto it = m_pShaderResources.find(key); it != m_pShaderResources.end())
        {
            return it->second.get();
        }
    }
    else if constexpr (std::same_as<T, MaterialResource>)
    {
        if (auto it = m_pMaterialResources.find(key); it != m_pMaterialResources.end())
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
