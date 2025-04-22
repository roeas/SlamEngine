#include "VertexArray.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGlVertexArray.h"
#include "Renderer/RenderCore.h"

namespace sl
{

VertexArray *VertexArray::Create(std::unique_ptr<sl::VertexBuffer> pVertexBuffer,
    std::unique_ptr<sl::IndexBuffer> pIndexBuffer, VertexLayout vertexLayout)
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
            return new OpenGLVertexArray{ std::move(pVertexBuffer), std::move(pIndexBuffer), std::move(vertexLayout) };
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
