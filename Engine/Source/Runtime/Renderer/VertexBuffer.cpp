#include "VertexBuffer.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLVertexBuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

VertexBuffer *VertexBuffer::Create(const float *pVertices, size_t size)
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
            return new OpenGLVertexBuffer{ pVertices, size };
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
