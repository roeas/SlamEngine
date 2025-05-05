#include "Resource.h"

#include "Core/Log.h"

namespace sl
{

void Resource::Update()
{
    switch (m_state)
    {
        case ResourceState::Importing:
        {
            OnImport();
            break;
        }
        case ResourceState::Building:
        {
            OnBuild();
            break;
        }
        case ResourceState::Loading:
        {
            OnLoad();
            break;
        }
        case ResourceState::Uploading:
        {
            OnUpload();
            break;
        }
        case ResourceState::Ready:
        {
            OnReady();
            break;
        }
        case ResourceState::Destroying:
        {
            OnDestroy();
            break;
        }
        case ResourceState::Destroyed:
        {
            break;
        }
        default:
        {
            SL_ASSERT(false, "Unknown resource state!");
            break;
        }
    }
}

} // namespace sl
