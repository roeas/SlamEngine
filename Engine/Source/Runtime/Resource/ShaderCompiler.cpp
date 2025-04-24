#include "ShaderCompiler.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Renderer//RenderCore.h"
#include "Utils/FileIO.hpp"

#include <shaderc/shaderc.hpp>

namespace sl
{

namespace
{

constexpr const char *BackendToDefinition[nameof::enum_count<GraphicsBackend>()] =
{
    "SL_NONE",      // GraphicsBackend::None
    "SL_OPENGL",    // GraphicsBackend::OpenGL
    "SL_VULKAN",    // GraphicsBackend::Vulkan
    "SL_DIRECTX11", // GraphicsBackend::DirectX11
    "SL_DIRECTX12", // GraphicsBackend::DirectX12
    "SL_METAL",     // GraphicsBackend::Metal
};

constexpr shaderc_shader_kind ShaderTypeToShaderKind[nameof::enum_count<ShaderType>()] =
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
        std::filesystem::path path = type == shaderc_include_type_standard ?
            Path::FromeAsset("Shader/Lib") : Path::FromeAsset("Shader");

        path /= requested_source;
        m_pathContainer = path.generic_string();
        m_contentContainer = FileIO::ReadString(m_pathContainer);

        auto *pResult = new shaderc_include_result;
        pResult->source_name = m_pathContainer.data();
        pResult->source_name_length = m_pathContainer.size();
        pResult->content = m_contentContainer.data();
        pResult->content_length = m_contentContainer.size();

        return pResult;
    }

    void ReleaseInclude(shaderc_include_result *pResult) override
    {
        delete pResult;
    }

private:
    // To ensure requested data is valid before calling ReleaseInclude
    std::string m_pathContainer;
    std::string m_contentContainer;
};

} // namespace

std::vector<uint32_t> ShaderCompiler::SourceToSpirv(const ShaderInfo &info)
{
    SL_LOG_TRACE("Compiling SPIR-V {}", info.m_name.data());

    std::string preprocessedShaderSource;
    shaderc_shader_kind shaderKind = ShaderTypeToShaderKind[(size_t)info.m_type];

    // 1. Preprocess
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        options.SetIncluder(std::make_unique<ShaderIncluder>());
        options.AddMacroDefinition(BackendToDefinition[(size_t)RenderCore::GetBackend()]);

        auto result = compiler.PreprocessGlsl(info.m_source, shaderKind, info.m_name.data(), options);
        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            SL_LOG_ERROR("Failed to preprocess shader: {}", result.GetErrorMessage());
            return {};
        }

        preprocessedShaderSource = { result.cbegin(), result.cend() };
    }

    // 2. Compile
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        if (info.m_debugMode)
        {
            options.SetGenerateDebugInfo();
            options.SetOptimizationLevel(shaderc_optimization_level_zero);
        }
        else
        {
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
        }

        options.SetSourceLanguage(shaderc_source_language_glsl);
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
            preprocessedShaderSource.data(), preprocessedShaderSource.size(),
            shaderKind, info.m_name.data(), "main", options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            SL_LOG_ERROR("Failed to compile SPIR-V: {}", result.GetErrorMessage());
            return {};
        }

        return { result.cbegin(), result.cend() };
    }
}

std::string ShaderCompiler::SpirvToSource(std::vector<uint32_t> spirv)
{
    return {};
}

void ShaderCompiler::Reflect(const std::vector<uint32_t> &spirv)
{

}

} // namespace sl
