#include "Framebuffer.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLFramebuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

Framebuffer *Framebuffer::Create(std::initializer_list<Texture2D *> pTextures, bool destroy)
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
            return new OpenGLFramebuffer{ pTextures };
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
