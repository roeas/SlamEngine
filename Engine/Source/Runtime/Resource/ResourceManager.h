#pragma once

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

private:
    template<class T>
    static void AddResource(StringHashType key, T *pResource);
    template<class T>
    static T *GetResource(StringHashType key);
};

} // namespace sl
