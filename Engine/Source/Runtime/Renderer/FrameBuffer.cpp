#include "FrameBuffer.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

FrameBuffer *FrameBuffer::Create(std::vector<Texture2D *> pTextures, bool destroy)
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
            return new OpenGLFrameBuffer{ std::move(pTextures) };
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
