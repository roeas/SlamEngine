#pragma once

#include "Core/Defines.h"
#include "Utils/EnumOf.hpp"

#include <glad/gl.h>

namespace sl
{

constexpr GLenum GLAttribType[nameof::enum_count<AttribType>()] =
{
    GL_BYTE,           // AttribType::Int8
    GL_UNSIGNED_BYTE,  // AttribType::Uint8
    GL_SHORT,          // AttribType::Int16
    GL_UNSIGNED_SHORT, // AttribType::Uint16
    GL_INT,            // AttribType::Int32
    GL_UNSIGNED_INT,   // AttribType::Uint32
    GL_HALF_FLOAT,     // AttribType::Half
    GL_FLOAT,          // AttribType::Float
    GL_DOUBLE,         // AttribType::Double
};

constexpr GLenum GLShaderType[nameof::enum_count<ShaderType>()] =
{
    GL_VERTEX_SHADER,   // ShaderType::VertexShader
    GL_FRAGMENT_SHADER, // ShaderType::FragmentShader
    GL_COMPUTE_SHADER,  // ShaderType::ComputeShader
};

} // namespace sl
