#include "VertexArray.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGlVertexArray.h"
#include "Renderer/RenderCore.h"

namespace sl
{

VertexArray *VertexArray::Create(sl::VertexBuffer *pVertexBuffer, sl::IndexBuffer *pIndexBuffer, const VertexLayout &vertexLayout)
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
            return new OpenGLVertexArray{ pVertexBuffer, pIndexBuffer, vertexLayout };
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
