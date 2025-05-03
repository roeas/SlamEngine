#include "ShaderCompiler.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Renderer/RenderCore.h"
#include "Utils/FileIO.hpp"
#include "Utils/ProfilerCPU.h"

#include <magic_enum/magic_enum.hpp>
#include <shaderc/shaderc.hpp>

namespace sl
{

namespace
{

constexpr const char *BackendToDefinition[magic_enum::enum_count<GraphicsBackend>()] =
{
    "SL_NONE",      // GraphicsBackend::None
    "SL_OPENGL",    // GraphicsBackend::OpenGL
    "SL_VULKAN",    // GraphicsBackend::Vulkan
    "SL_DIRECTX11", // GraphicsBackend::DirectX11
    "SL_DIRECTX12", // GraphicsBackend::DirectX12
    "SL_METAL",     // GraphicsBackend::Metal
};

constexpr shaderc_shader_kind ShaderTypeToShaderKind[magic_enum::enum_count<ShaderType>()] =
{
    shaderc_vertex_shader,   // ShaderType::VertexShader
    shaderc_fragment_shader, // ShaderType::FragmentShader
    shaderc_compute_shader,  // ShaderType::ComputeShader
};

// Shaderc includer
class ShaderIncluder : public shaderc::CompileOptions::IncluderInterface
{
public:
    shaderc_include_result *GetInclude(const char *requested_source, shaderc_include_type type,
        const char *requesting_source, size_t include_depth) override
    {
        SL_LOG_TRACE("\tIncluder requesting {}", requesting_source);
        SL_LOG_TRACE("\tIncluder requested {}", Path::NameWithExtension(requested_source).data());
        SL_LOG_TRACE("\tIncluder depth {}", include_depth);

        std::filesystem::path path = type == shaderc_include_type_standard ?
            Path::FromeAsset("Shader/Lib") : Path::FromeAsset("Shader");
        path /= requested_source;

        auto pContainer = new std::pair<std::string, std::string>;
        pContainer->first = requested_source;
        pContainer->second = FileIO::ReadString(path.generic_string().data());

        auto *pResult = new shaderc_include_result;
        pResult->user_data = pContainer;
        pResult->source_name = pContainer->first.data();
        pResult->source_name_length = pContainer->first.size();
        pResult->content = pContainer->second.data();
        pResult->content_length = pContainer->second.size();

        return pResult;
    }

    void ReleaseInclude(shaderc_include_result *pResult) override
    {
        delete static_cast<std::pair<std::string, std::string> *>(pResult->user_data);
        delete pResult;
    }
};

} // namespace

std::vector<uint32_t> ShaderCompiler::SourceToSpirv(const ShaderInfo &info)
{
    SL_PROFILE;

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    shaderc_shader_kind shaderKind = ShaderTypeToShaderKind[(size_t)info.m_type];

    // Preprocesser options
    options.SetIncluder(std::make_unique<ShaderIncluder>());
    options.AddMacroDefinition(BackendToDefinition[(size_t)RenderCore::GetBackend()]);

    // Compiler options
    if (info.m_debugMode)
    {
        options.SetGenerateDebugInfo();
        options.SetOptimizationLevel(shaderc_optimization_level_zero);
    }
    else
    {
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    // TODO: RHI
    options.SetSourceLanguage(shaderc_source_language_glsl);
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

    auto result = compiler.CompileGlslToSpv(info.m_source, shaderKind, info.m_name.data(), "main", options);
    if (result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        SL_LOG_ERROR("Failed to compile SPIR-V: {}", result.GetErrorMessage());
        return {};
    }

    return { result.cbegin(), result.cend() };
}

std::string ShaderCompiler::SpirvToSource(std::vector<uint32_t> spirv)
{
    return {};
}

void ShaderCompiler::Reflect(const std::vector<uint32_t> &spirv)
{

}

} // namespace sl
