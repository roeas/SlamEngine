#include "Shader.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include "Renderer/RenderCore.h"

namespace sl
{

namespace
{

OpenGLShader *CreateWrapper(auto fun)
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
            return fun();
            break;
        }
        default:
        {
            SL_ASSERT(false);
            return nullptr;
        }
    }
}

} // namespace

Shader *Shader::Create(std::string_view vsSource, std::string_view fsSource)
{
    return CreateWrapper([vsSource, fsSource]()
    {
        return new OpenGLShader{ vsSource, fsSource };
    });
}

Shader *Shader::Create(std::string_view shaderSource, ShaderType type)
{
    return CreateWrapper([shaderSource, type]()
    {
        return new OpenGLShader{ shaderSource, type };
    });
}

Shader *Shader::Create(std::span<const uint32_t> vsBinary, std::span<const uint32_t> fsBinary)
{
    return CreateWrapper([vsBinary, fsBinary]()
    {
        return new OpenGLShader{ vsBinary, fsBinary };
    });
}

Shader *Shader::Create(std::span<const uint32_t> binary, ShaderType type)
{
    return CreateWrapper([binary, type]()
    {
        return new OpenGLShader{ binary, type };
    });
}

} // namespace sl
