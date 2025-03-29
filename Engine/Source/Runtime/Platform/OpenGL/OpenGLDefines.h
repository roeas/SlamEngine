#pragma once

#include "Core/Defines.h"
#include "Utils/EnumOf.hpp"

#include <glad/gl.h>

namespace sl
{

constexpr GLenum GLShaderType[nameof::enum_count<ShaderType>()] =
{
    GL_VERTEX_SHADER,   // ShaderType::VertexShader
    GL_FRAGMENT_SHADER, // ShaderType::FragmentShader
    GL_COMPUTE_SHADER,  // ShaderType::ComputeShader
};

} // namespace sl
