#include "Resource.h"

#include "Core/Log.h"

namespace sl
{

Resource::Resource(std::string assetPath, std::string internalPath) :
    m_assettPath(std::move(assetPath)), m_internalPath(std::move(internalPath))
{

}

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
