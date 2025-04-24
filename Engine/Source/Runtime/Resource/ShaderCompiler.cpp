#include "ShaderCompiler.h"

#include "Core/Log.h"

#include <shaderc/shaderc.hpp>

namespace sl
{

namespace
{

constexpr shaderc_shader_kind ShaderTypeToShaderKind[nameof::enum_count<ShaderType>()] =
{
    shaderc_vertex_shader,   // ShaderType::VertexShader
    shaderc_fragment_shader, // ShaderType::FragmentShader
    shaderc_compute_shader,  // ShaderType::ComputeShader
};

} // namespace

std::vector<uint32_t> ShaderCompiler::SourceToSpirv(const ShaderInfo &info)
{
    SL_LOG_TRACE("Compiling SPIR-V {}", info.m_name.data());

    shaderc_shader_kind shaderKind = ShaderTypeToShaderKind[(size_t)info.m_type];

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    options.SetGenerateDebugInfo();
    options.SetOptimizationLevel(info.m_debugMode ? shaderc_optimization_level_zero : shaderc_optimization_level_performance);

    options.SetSourceLanguage(shaderc_source_language_glsl);
    options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

    shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
        info.m_source.data(), info.m_source.size(),
        shaderKind, info.m_name.data(), "main", options);

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
