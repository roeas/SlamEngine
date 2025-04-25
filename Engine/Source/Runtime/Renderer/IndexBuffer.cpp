#include "IndexBuffer.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLIndexBuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

IndexBuffer *IndexBuffer::Create(std::span<const uint32_t> indices)
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
            return new OpenGLIndexBuffer{ indices };
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
