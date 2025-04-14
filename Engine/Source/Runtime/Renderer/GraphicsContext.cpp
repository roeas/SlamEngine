#include "GraphicsContext.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Renderer/RenderCore.h"

namespace sl
{

GraphicsContext *GraphicsContext::Create(void *pWindow)
{
    switch (RenderCore::GetBackend())
    {
        case GraphicsBackend::None:
        {
            SL_ASSERT(false);
            return nullptr;
        }
        case GraphicsBackend::OpenGL:
        {
            return new OpenGLContext{ pWindow };
            break;
        }
        default:
        {
            SL_ASSERT(false);
            return nullptr;
        }
    }
}

} // namespace sl
