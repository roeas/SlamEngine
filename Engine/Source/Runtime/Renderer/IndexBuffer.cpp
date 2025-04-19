#include "IndexBuffer.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLIndexBuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

IndexBuffer *IndexBuffer::Create(const uint32_t *pIndicies, size_t size)
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
            return new OpenGLIndexBuffer{ pIndicies, size };
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
