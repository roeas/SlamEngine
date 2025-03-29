#include "RenderContext.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Render/RenderCore.h"

namespace sl
{

RenderContext *RenderContext::Create(void *pWindow)
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
