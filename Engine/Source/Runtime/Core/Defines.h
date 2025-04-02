#pragma once

#include <cstdint>

namespace sl
{

enum class GraphicsBackend : uint8_t
{
    None = 0,
    OpenGL,
    Vulkan,
    DirectX11,
    DirectX12,
    Metal,
};

enum class AttribType : uint8_t
{
    Int8 = 0,
    Uint8,
    Int16,
    Uint16,
    Int32,
    Uint32,
    Half,
    Float,
    Double,
};

enum class ShaderType : uint8_t
{
    VertexShader = 0,
    FragmentShader,
    ComputeShader,
};

} // namespace sl
