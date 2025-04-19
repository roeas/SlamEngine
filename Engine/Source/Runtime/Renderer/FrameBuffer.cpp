#include "FrameBuffer.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

FrameBuffer *FrameBuffer::Create(std::initializer_list<Texture2D *> pTextures, bool destroy)
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
            return new OpenGLFrameBuffer{ pTextures };
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
