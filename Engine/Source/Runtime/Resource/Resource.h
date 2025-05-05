#pragma once

#include <cstdint>
#include <string>

namespace sl
{

enum class ResourceState : uint8_t
{
    Importing = 0,
    Building,
    Loading,
    Uploading,
    Ready,
    Destroying,
    Destroyed,
};

enum class ResourcesType : uint8_t
{
    Mesh = 0,
    Bone,
    Animation,
    Texture,
    Shader,
    Material,
};

class Resource
{
public:
    virtual ~Resource() = default;

    void Update();
    bool IsReady() { return m_state == ResourceState::Ready; }

protected:
    // Import asset file from original format
    virtual void OnImport() = 0;
    // Compile to internal format
    virtual void OnBuild() = 0;
    // Read serialized internal format file
    virtual void OnLoad() = 0;
    // Creat GPU handle
    virtual void OnUpload() = 0;
    // Destroy CPU data deferred
    virtual void OnReady() = 0;
    // Destroy both CPU and GPU data
    virtual void OnDestroy() = 0;
    // Destroy CPU data
    virtual void DestroyCPUData() = 0;

    ResourceState m_state = ResourceState::Importing;
    uint8_t m_destroyDelayFrame = 60;
    bool m_optimized = false;
};

} // namespace sl
