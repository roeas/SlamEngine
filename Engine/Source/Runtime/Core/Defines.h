#pragma once

#include <magic_enum/magic_enum.hpp>

namespace sl
{

enum class GraphicsBackend : uint8_t
{
    None,
    OpenGL,
    Vulkan,
    DirectX11,
    DirectX12,
    Metal,
};

enum class Function : uint8_t
{
    Never,
    Less,
    Greater,
    Equal,
    LessEqual,
    GreaterEqual,
    NotEqual,
    Always,
};

enum class Operation : uint8_t
{
    Keep,
    Zero,
    Replace,
    Increase,
    IncreaseWarp,
    Decrease,
    DecreaseWarp,
    Invert,
};

enum class Factor : uint8_t
{
    Zero,
    One,
    SourceColor,
    OneMinusSourceColor,
    DestinationColor,
    OneMinusDestinationColor,
    SourceAlpha,
    OneMinusSourceAlpha,
    DestinationAlpha,
    OneMinusDestinationAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
};

enum class Face : uint8_t
{
    Front,
    Back,
    FrontAndBack,
};

enum class Winding : uint8_t
{
    Clockwise,
    Counterclockwise,
};

enum class AttribType : uint8_t
{
    Int8,
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
    R8,  // Unsigned normalized integer
    R8S, // Signed normalized integer
    R8U, // Unsigned integer
    R8I, // Signed integer
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

constexpr AttachmentType TextureFormatToAttachmentType[magic_enum::enum_count<TextureFormat>()]
{
    AttachmentType::Color,           // TextureFormat::R8
    AttachmentType::Color,           // TextureFormat::R8S
    AttachmentType::Color,           // TextureFormat::R8U
    AttachmentType::Color,           // TextureFormat::R8I
    AttachmentType::Color,           // TextureFormat::R16
    AttachmentType::Color,           // TextureFormat::R16S
    AttachmentType::Color,           // TextureFormat::R16U
    AttachmentType::Color,           // TextureFormat::R16I
    AttachmentType::Color,           // TextureFormat::R16F
    AttachmentType::Color,           // TextureFormat::R32U
    AttachmentType::Color,           // TextureFormat::R32I
    AttachmentType::Color,           // TextureFormat::R32F
    AttachmentType::Color,           // TextureFormat::RG8
    AttachmentType::Color,           // TextureFormat::RG8S
    AttachmentType::Color,           // TextureFormat::RG8U
    AttachmentType::Color,           // TextureFormat::RG8I
    AttachmentType::Color,           // TextureFormat::RG16
    AttachmentType::Color,           // TextureFormat::RG16S
    AttachmentType::Color,           // TextureFormat::RG16U
    AttachmentType::Color,           // TextureFormat::RG16I
    AttachmentType::Color,           // TextureFormat::RG16F
    AttachmentType::Color,           // TextureFormat::RG32U
    AttachmentType::Color,           // TextureFormat::RG32I
    AttachmentType::Color,           // TextureFormat::RG32F
    AttachmentType::Color,           // TextureFormat::RGB8
    AttachmentType::Color,           // TextureFormat::RGB8S
    AttachmentType::Color,           // TextureFormat::RGB8U
    AttachmentType::Color,           // TextureFormat::RGB8I
    AttachmentType::Color,           // TextureFormat::RGB16
    AttachmentType::Color,           // TextureFormat::RGB16S
    AttachmentType::Color,           // TextureFormat::RGB16U
    AttachmentType::Color,           // TextureFormat::RGB16I
    AttachmentType::Color,           // TextureFormat::RGB16F
    AttachmentType::Color,           // TextureFormat::RGB32U
    AttachmentType::Color,           // TextureFormat::RGB32I
    AttachmentType::Color,           // TextureFormat::RGB32F
    AttachmentType::Color,           // TextureFormat::RGBA8
    AttachmentType::Color,           // TextureFormat::RGBA8S
    AttachmentType::Color,           // TextureFormat::RGBA8U
    AttachmentType::Color,           // TextureFormat::RGBA8I
    AttachmentType::Color,           // TextureFormat::RGBA16
    AttachmentType::Color,           // TextureFormat::RGBA16S
    AttachmentType::Color,           // TextureFormat::RGBA16U
    AttachmentType::Color,           // TextureFormat::RGBA16I
    AttachmentType::Color,           // TextureFormat::RGBA16F
    AttachmentType::Color,           // TextureFormat::RGBA32U
    AttachmentType::Color,           // TextureFormat::RGBA32I
    AttachmentType::Color,           // TextureFormat::RGBA32F
    AttachmentType::Depth,           // TextureFormat::D16
    AttachmentType::Depth,           // TextureFormat::D24
    AttachmentType::Depth,           // TextureFormat::D32
    AttachmentType::Depth,           // TextureFormat::D32F
    AttachmentType::Stencil,         // TextureFormat::S8
    AttachmentType::DepthAndStencil, // TextureFormat::D24S8
    AttachmentType::DepthAndStencil, // TextureFormat::D32FS8
};

enum class ShaderType : uint8_t
{
    VertexShader,
    FragmentShader,
    ComputeShader,
};

} // namespace sl

// Clear
#define SL_CLEAR_NONE          UINT8_C(0x00)

#define SL_CLEAR_COLOR         UINT8_C(0x01)
#define SL_CLEAR_COLOR_MASK    UINT8_C(0x01)
#define SL_CLEAR_COLOR_SHIFT   0

#define SL_CLEAR_DEPTH         UINT8_C(0x02)
#define SL_CLEAR_DEPTH_MASK    UINT8_C(0x02)
#define SL_CLEAR_DEPTH_SHIFT   0

#define SL_CLEAR_STENCIL       UINT8_C(0x04)
#define SL_CLEAR_STENCIL_MASK  UINT8_C(0x04)
#define SL_CLEAR_STENCIL_SHIFT 0

#define SL_CLEAR_ALL (0 \
    | SL_CLEAR_COLOR \
    | SL_CLEAR_DEPTH \
    | SL_CLEAR_STENCIL \
    )

// Sampler
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
