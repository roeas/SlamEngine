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

enum class TextureFormat : uint8_t
{
    R8 = 0, // Unsigned normalized integer
    R8S,    // Signed normalized integer
    R8U,    // Unsigned integer
    R8I,    // Signed integer
    R16,
    R16S,
    R16U,
    R16I,
    R16F,
    R32U,
    R32I,
    R32F,

    RG8,
    RG8S,
    RG8U,
    RG8I,
    RG16,
    RG16S,
    RG16U,
    RG16I,
    RG16F,
    RG32U,
    RG32I,
    RG32F,

    RGB8,
    RGB8S,
    RGB8U,
    RGB8I,
    RGB16,
    RGB16S,
    RGB16U,
    RGB16I,
    RGB16F,
    RGB32U,
    RGB32I,
    RGB32F,

    RGBA8,
    RGBA8S,
    RGBA8U,
    RGBA8I,
    RGBA16,
    RGBA16S,
    RGBA16U,
    RGBA16I,
    RGBA16F,
    RGBA32U,
    RGBA32I,
    RGBA32F,

    D16,
    D24,
    D32,
    D32F,
    D24S8,
    D32FS8,
    S8,

    // TODO: Compress format
};

enum class ShaderType : uint8_t
{
    VertexShader = 0,
    FragmentShader,
    ComputeShader,
};

} // namespace sl
