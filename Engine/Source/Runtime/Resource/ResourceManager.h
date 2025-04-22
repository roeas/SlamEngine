#pragma once

#include "Resource/MeshResource.h"
#include "Resource/TextureResource.h"
#include "Utils/Hash.hpp"

#include <map>
#include <memory>

namespace sl
{

class ResourceManager final
{
public:
    ResourceManager() = delete;
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;
    ~ResourceManager() = delete;

    static void Update();

    static void AddMeshResource(StringHashType key, std::unique_ptr<MeshResource> pResource);
    static MeshResource *GetMeshResource(StringHashType key);

    static void AddTextureResource(StringHashType key, std::unique_ptr<TextureResource> pResource);
    static TextureResource *GetTextureResource(StringHashType key);

private:
    template<class T>
    static void AddResource(StringHashType key, std::unique_ptr<T> pResource);
    template<class T>
    static T *GetResource(StringHashType key);

    static inline std::map<StringHashType, std::unique_ptr<MeshResource>> m_pMeshResources;
    static inline std::map<StringHashType, std::unique_ptr<TextureResource>> m_pTextureResources;
};

} // namespace sl
