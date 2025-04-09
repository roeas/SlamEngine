#pragma once

#include "Utils/EnumOf.hpp"

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
    S8,
    D24S8,
    D32FS8,

    // TODO: Compress format
};

enum class AttachmentType : uint8_t
{
    Color,
    Depth,
    Stencil,
    DepthAndStencil,
};

constexpr AttachmentType TextureFormatToAttachmentType[nameof::enum_count<TextureFormat>()] =
{
    AttachmentType::Color,           // R8
    AttachmentType::Color,           // R8S
    AttachmentType::Color,           // R8U
    AttachmentType::Color,           // R8I
    AttachmentType::Color,           // R16
    AttachmentType::Color,           // R16S
    AttachmentType::Color,           // R16U
    AttachmentType::Color,           // R16I
    AttachmentType::Color,           // R16F
    AttachmentType::Color,           // R32U
    AttachmentType::Color,           // R32I
    AttachmentType::Color,           // R32F
    AttachmentType::Color,           // RG8
    AttachmentType::Color,           // RG8S
    AttachmentType::Color,           // RG8U
    AttachmentType::Color,           // RG8I
    AttachmentType::Color,           // RG16
    AttachmentType::Color,           // RG16S
    AttachmentType::Color,           // RG16U
    AttachmentType::Color,           // RG16I
    AttachmentType::Color,           // RG16F
    AttachmentType::Color,           // RG32U
    AttachmentType::Color,           // RG32I
    AttachmentType::Color,           // RG32F
    AttachmentType::Color,           // RGB8
    AttachmentType::Color,           // RGB8S
    AttachmentType::Color,           // RGB8U
    AttachmentType::Color,           // RGB8I
    AttachmentType::Color,           // RGB16
    AttachmentType::Color,           // RGB16S
    AttachmentType::Color,           // RGB16U
    AttachmentType::Color,           // RGB16I
    AttachmentType::Color,           // RGB16F
    AttachmentType::Color,           // RGB32U
    AttachmentType::Color,           // RGB32I
    AttachmentType::Color,           // RGB32F
    AttachmentType::Color,           // RGBA8
    AttachmentType::Color,           // RGBA8S
    AttachmentType::Color,           // RGBA8U
    AttachmentType::Color,           // RGBA8I
    AttachmentType::Color,           // RGBA16
    AttachmentType::Color,           // RGBA16S
    AttachmentType::Color,           // RGBA16U
    AttachmentType::Color,           // RGBA16I
    AttachmentType::Color,           // RGBA16F
    AttachmentType::Color,           // RGBA32U
    AttachmentType::Color,           // RGBA32I
    AttachmentType::Color,           // RGBA32F
    AttachmentType::Depth,           // D16
    AttachmentType::Depth,           // D24
    AttachmentType::Depth,           // D32
    AttachmentType::Depth,           // D32F
    AttachmentType::Stencil,         // S8
    AttachmentType::DepthAndStencil, // D24S8
    AttachmentType::DepthAndStencil, // D32FS8
};

enum class ShaderType : uint8_t
{
    VertexShader = 0,
    FragmentShader,
    ComputeShader,
};

} // namespace sl

#define SL_SAMPLER_NONE                   UINT32_C(0x00000000)

#define SL_SAMPLER_U_REPEAT               UINT32_C(0x00000001)
#define SL_SAMPLER_U_MIRROR               UINT32_C(0x00000002)
#define SL_SAMPLER_U_CLAMP                UINT32_C(0x00000003)
#define SL_SAMPLER_U_BORDER               UINT32_C(0x00000004)
#define SL_SAMPLER_U_MASK                 UINT32_C(0x0000000f)
#define SL_SAMPLER_U_SHIFT                0

#define SL_SAMPLER_V_REPEAT               UINT32_C(0x00000010)
#define SL_SAMPLER_V_MIRROR               UINT32_C(0x00000020)
#define SL_SAMPLER_V_CLAMP                UINT32_C(0x00000030)
#define SL_SAMPLER_V_BORDER               UINT32_C(0x00000040)
#define SL_SAMPLER_V_MASK                 UINT32_C(0x000000f0)
#define SL_SAMPLER_V_SHIFT                4

#define SL_SAMPLER_W_REPEAT               UINT32_C(0x00000100)
#define SL_SAMPLER_W_MIRROR               UINT32_C(0x00000200)
#define SL_SAMPLER_W_CLAMP                UINT32_C(0x00000300)
#define SL_SAMPLER_W_BORDER               UINT32_C(0x00000400)
#define SL_SAMPLER_W_MASK                 UINT32_C(0x00000f00)
#define SL_SAMPLER_W_SHIFT                8

#define SL_SAMPLER_MIN_NEAREST            UINT32_C(0x00001000)
#define SL_SAMPLER_MIN_LINEAR             UINT32_C(0x00002000)
#define SL_SAMPLER_MIN_MASK               UINT32_C(0x00003000)
#define SL_SAMPLER_MIN_SHIFT              12

#define SL_SAMPLER_MAG_NEAREST            UINT32_C(0x00004000)
#define SL_SAMPLER_MAG_LINEAR             UINT32_C(0x00008000)
#define SL_SAMPLER_MAG_MASK               UINT32_C(0x0000c000)
#define SL_SAMPLER_MAG_SHIFT              14

#define SL_SAMPLER_MIPMAP_NEAREST_NEAREST UINT32_C(0x00010000) // Nearest between pixels, nearest between mipmaps
#define SL_SAMPLER_MIPMAP_NEAREST_LINEAR  UINT32_C(0x00030000) // Nearest between pixels, linear between mipmaps
#define SL_SAMPLER_MIPMAP_LINEAR_NEAREST  UINT32_C(0x00020000) // Linear between pixels, nearest between mipmaps
#define SL_SAMPLER_MIPMAP_LINEAR_LINEAR   UINT32_C(0x00040000) // Linear between pixels, linear between mipmaps
#define SL_SAMPLER_MIPMAP_MASK            UINT32_C(0x000f0000)
#define SL_SAMPLER_MIPMAP_SHIFT           16

#define SL_SAMPLER_REPEAT (0 \
    | SL_SAMPLER_U_REPEAT \
    | SL_SAMPLER_V_REPEAT \
    | SL_SAMPLER_W_REPEAT \
    )

#define SL_SAMPLER_MIRROR (0 \
    | SL_SAMPLER_U_MIRROR \
    | SL_SAMPLER_V_MIRROR \
    | SL_SAMPLER_W_MIRROR \
    )

#define SL_SAMPLER_CLAMP (0 \
    | SL_SAMPLER_U_CLAMP \
    | SL_SAMPLER_V_CLAMP \
    | SL_SAMPLER_W_CLAMP \
    )

#define SL_SAMPLER_BORDER (0 \
    | SL_SAMPLER_U_BORDER \
    | SL_SAMPLER_V_BORDER \
    | SL_SAMPLER_W_BORDER \
    )

#define SL_SAMPLER_NEAREST (0 \
    | SL_SAMPLER_MIN_NEAREST \
    | SL_SAMPLER_MAG_NEAREST \
    | SL_SAMPLER_MIPMAP_NEAREST_NEAREST \
    )

#define SL_SAMPLER_LINEAR (0 \
    | SL_SAMPLER_MIN_LINEAR \
    | SL_SAMPLER_MAG_LINEAR \
    | SL_SAMPLER_MIPMAP_LINEAR_LINEAR \
    )
