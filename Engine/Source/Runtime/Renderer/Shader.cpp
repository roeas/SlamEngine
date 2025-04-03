#include "Shader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/RenderCore.h"

namespace sl
{

Shader *Shader::Create(std::string_view vsSource, std::string_view fsSource)
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
            return new OpenGLShader{ vsSource, fsSource };
            break;
        }
        default:
        {
            SL_ASSERT(false);
            return nullptr;
        }
    }
}

Shader *Shader::Create(std::string_view shaderSource, ShaderType type)
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
            return new OpenGLShader{ shaderSource, type };
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
