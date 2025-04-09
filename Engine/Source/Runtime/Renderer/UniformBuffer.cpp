#include "UniformBuffer.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Renderer/RenderCore.h"

namespace sl
{

UniformBuffer *UniformBuffer::Create(uint32_t bindignPoint, UniformBufferLayout layout)
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
            return new OpenGLUniformBuffer{ bindignPoint, std::move(layout) };
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
