#include "Texture.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLTexture.h"
#include "Renderer/RenderCore.h"

namespace sl
{

Texture2D *Texture2D::Create(uint32_t width, uint32_t height, TextureFormat format, bool mipmap, uint32_t flags, const void *pData)
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
            return new OpenGLTexture2D{ width, height, format, mipmap, flags, pData };
            break;
        }
        default:
        {
            SL_ASSERT(false);
            return nullptr;
        }
    }
}

TextureCube *TextureCube::Create(uint32_t width, uint32_t height, TextureFormat format, bool genMipmap, uint32_t flags, std::vector<std::vector<const void *>> pDatas)
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
            return new OpenGLTextureCube{ width, height, format, genMipmap, flags, std::move(pDatas) };
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
