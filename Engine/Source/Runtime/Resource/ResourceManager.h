#pragma once

#include "Resource/MaterialResource.h"
#include "Resource/MeshResource.h"
#include "Resource/ShaderResource.h"
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

    static void AddShaderResource(StringHashType key, std::unique_ptr<ShaderResource> pResource);
    static ShaderResource *GetShaderResource(StringHashType key);

    static void AddMaterialResource(StringHashType key, std::unique_ptr<MaterialResource> pResource);
    static MaterialResource *GetMaterialResource(StringHashType key);

private:
    template<class T>
    static void AddResource(StringHashType key, std::unique_ptr<T> pResource);
    template<class T>
    static T *GetResource(StringHashType key);

    static inline std::map<StringHashType, std::unique_ptr<MeshResource>> m_pMeshResources;
    static inline std::map<StringHashType, std::unique_ptr<TextureResource>> m_pTextureResources;
    static inline std::map<StringHashType, std::unique_ptr<ShaderResource>> m_pShaderResources;
    static inline std::map<StringHashType, std::unique_ptr<MaterialResource>> m_pMaterialResources;
};

} // namespace sl
