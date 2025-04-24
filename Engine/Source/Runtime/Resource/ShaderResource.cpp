#include "ShaderResource.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Renderer/Shader.h"
#include "resource/ShaderCompiler.h"
#include "Utils/FileIO.hpp"
#include "Utils/Time.h"

namespace sl
{

namespace
{

consteval bool IsShaderDebugMode()
{
#if defined(SL_DEBUF)
    return true;
#else
    return false;
#endif
}

std::string GetShaderBinaryCachePath(std::string_view name)
{
    std::filesystem::path path = Path::FromeRoot("Engine/Binary");
    path /= name;
    if (IsShaderDebugMode())
    {
        path += "d";
    }
    path.replace_extension(".spv");

    return path.generic_string();
}

} // namespace

ShaderResource::ShaderResource(std::string_view vsPath, std::string_view fsPath) :
    m_shaderCount(2)
{
    m_shaders[0].m_name = Path::NameWithoutExtension(vsPath);
    m_shaders[0].m_assetPath = vsPath;
    m_shaders[0].m_binaryPath = GetShaderBinaryCachePath(m_shaders[0].m_name);
    m_shaders[0].m_type = ShaderType::VertexShader;
    m_shaders[0].m_debugMode = IsShaderDebugMode();

    m_shaders[1].m_name = Path::NameWithoutExtension(fsPath);
    m_shaders[1].m_assetPath = fsPath;
    m_shaders[1].m_binaryPath = GetShaderBinaryCachePath(m_shaders[1].m_name);
    m_shaders[1].m_type = ShaderType::FragmentShader;
    m_shaders[1].m_debugMode = IsShaderDebugMode();

    /*
     * We assume that a shader is named by its shader program name plus the stage suffix
     * Shader Program: XXX
     * Vertex Shader: XXX_vert.glsl
     * Fragment Shader: XXX_frag.glsl
     */
    m_name = m_shaders[0].m_name.substr(0, m_shaders[0].m_name.rfind('_'));

    m_state = ResourceState::Importing;
}

ShaderResource::ShaderResource(std::string_view path, ShaderType type) :
    m_shaderCount(1)
{
    m_shaders[0].m_name = Path::NameWithExtension(path);
    m_shaders[0].m_assetPath = path;
    m_shaders[0].m_binaryPath = GetShaderBinaryCachePath(m_shaders[0].m_name);
    m_shaders[0].m_type = type;
    m_shaders[0].m_debugMode = IsShaderDebugMode();

    m_name = m_shaders[0].m_name.substr(0, m_shaders[0].m_name.rfind('_'));

    m_state = ResourceState::Importing;
}

ShaderResource::~ShaderResource()
{
    OnDestroy();
}

void ShaderResource::OnImport()
{
    SL_LOG_TRACE("Loading shader \"{}\"", m_shaders[0].m_assetPath.data());
    auto optShaderSource = FileIO::ReadString(m_shaders[0].m_assetPath);
    if (optShaderSource)
    {
        m_shaders[0].m_source = std::move(optShaderSource.value());
    }

    if (m_shaderCount == 2)
    {
        SL_LOG_TRACE("Loading shader \"{}\"", m_shaders[1].m_assetPath.c_str());
        auto optShaderSource1 = FileIO::ReadString(m_shaders[1].m_assetPath);
        if (optShaderSource1)
        {
            m_shaders[1].m_source = std::move(optShaderSource1.value());
        }
    }

    if (m_shaders[0].m_source.empty() || (m_shaderCount == 2 && m_shaders[1].m_source.empty()))
    {
        SL_LOG_ERROR("Failed to import shader!");
        m_state = ResourceState::Destroying;
        return;
    }

    m_state = ResourceState::Building;
}

void ShaderResource::OnBuild()
{
    Timer timer;
    auto spirvData = ShaderCompiler::SourceToSpirv(m_shaders[0]);
    FileIO::WriteBinary<decltype(spirvData)::value_type>(m_shaders[0].m_binaryPath, spirvData);
    //m_shaders[0].m_source = ShaderCompiler::SpirvToSource(std::move(spirvData));
    SL_LOG_TRACE("Done in {} ms", timer.GetDuration());

    if (m_shaderCount == 2)
    {
        timer.Reset();
        auto fragSpirvData = ShaderCompiler::SourceToSpirv(m_shaders[1]);
        FileIO::WriteBinary<decltype(fragSpirvData)::value_type>(m_shaders[1].m_binaryPath, fragSpirvData);
        //m_shaders[1].m_source = ShaderCompiler::SpirvToSource(std::move(fragSpirvData));
        SL_LOG_TRACE("Done in {} ms", timer.GetDuration());
    }

    if (m_shaders[0].m_source.empty() || (m_shaderCount == 2 && m_shaders[1].m_source.empty()))
    {
        SL_LOG_ERROR("Failed to compile shader {}", m_name.data());
        m_state = ResourceState::Destroying;
        return;
    }

    m_state = ResourceState::Uploading;
}

void ShaderResource::OnLoad()
{
    m_state = ResourceState::Uploading;
}

void ShaderResource::OnUpload()
{
    SL_LOG_TRACE("Uploading shader program \"{}\"", m_name.data());

    if (m_shaderCount == 2)
    {
        m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_source, m_shaders[1].m_source));
    }
    else
    {
        m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_source, m_shaders[0].m_type));
    }

    if (!m_pShaderProgram)
    {
        SL_LOG_ERROR("Failed to create shader program \"{}\" handle", m_name.data());
        m_state = ResourceState::Destroying;
        return;
    }

    m_state = ResourceState::Ready;
}

void ShaderResource::OnReady()
{
    if (!m_optimized) [[unlikely]]
    {
        if (!m_destroyDelayFrame--)
        {
            DestroyCPUData();
        }
    }
}

void ShaderResource::OnDestroy()
{
    DestroyCPUData();
    m_pShaderProgram.reset();
    m_state = ResourceState::Destroyed;
}

void ShaderResource::DestroyCPUData()
{
    m_shaders[0].m_source.clear();
    std::string{}.swap(m_shaders[0].m_source);

    if (m_shaderCount == 2)
    {
        m_shaders[1].m_source.clear();
        std::string{}.swap(m_shaders[1].m_source);
    }
}

} // namespace sl
