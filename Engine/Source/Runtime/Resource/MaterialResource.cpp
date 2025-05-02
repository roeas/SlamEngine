#include "MaterialResource.h"

namespace sl
{

MaterialResource::~MaterialResource()
{
    OnDestroy();
}

void MaterialResource::OnImport()
{
    m_state = ResourceState::Building;
}

void MaterialResource::OnBuild()
{
    m_state = ResourceState::Uploading;
}

void MaterialResource::OnLoad()
{
    m_state = ResourceState::Uploading;
}

void MaterialResource::OnUpload()
{
    m_state = ResourceState::Ready;
}

void MaterialResource::OnReady()
{
    if (!m_optimized) [[unlikely]]
    {
        if (!m_destroyDelayFrame--)
        {
            DestroyCPUData();
        }
    }
}

void MaterialResource::OnDestroy()
{
    DestroyCPUData();
    m_state = ResourceState::Destroyed;
}

void MaterialResource::DestroyCPUData()
{
    m_optimized = true;
}

} // namespace sl
