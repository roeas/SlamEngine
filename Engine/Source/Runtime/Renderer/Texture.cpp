#include "Texture.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer/RenderCore.h"

namespace sl
{

Texture2D *Texture2D::Create(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags, const void *pData)
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
            return new OpenGLTexture2D{ width, height, mipmap, format, flags, pData };
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
