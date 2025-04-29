#include "ShaderResource.h"

#include "Core/Log.h"
#include "Core/Path.h"
#include "Renderer/Shader.h"
#include "resource/ShaderCompiler.h"
#include "Utils/FileIO.hpp"
#include "Utils/ProfilerCPU.h"
#include "Utils/Time.h"

namespace sl
{

namespace
{

consteval bool IsShaderDebugMode()
{
#if defined(SL_DEBUG)
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

ShaderResource::ShaderResource(std::string vsPath, std::string fsPath) :
    m_shaderCount(2)
{
    m_shaders[0].m_name = Path::NameWithoutExtension(vsPath);
    m_shaders[0].m_assetPath = std::move(vsPath);
    m_shaders[0].m_internalPath = GetShaderBinaryCachePath(m_shaders[0].m_name);
    m_shaders[0].m_type = ShaderType::VertexShader;
    m_shaders[0].m_debugMode = IsShaderDebugMode();

    m_shaders[1].m_name = Path::NameWithoutExtension(fsPath);
    m_shaders[1].m_assetPath = std::move(fsPath);
    m_shaders[1].m_internalPath = GetShaderBinaryCachePath(m_shaders[1].m_name);
    m_shaders[1].m_type = ShaderType::FragmentShader;
    m_shaders[1].m_debugMode = IsShaderDebugMode();

    m_name = m_shaders[0].m_name.substr(0, m_shaders[0].m_name.rfind('_'));

    if (Path::Exists(m_shaders[0].m_internalPath) && Path::Exists(m_shaders[1].m_internalPath))
    {
        m_state = ResourceState::Loading;
    }
    else
    {
        m_state = ResourceState::Importing;
    }
}

ShaderResource::ShaderResource(std::string path, ShaderType type) :
    m_shaderCount(1)
{
    m_shaders[0].m_name = Path::NameWithExtension(path);
    m_shaders[0].m_assetPath = std::move(path);
    m_shaders[0].m_internalPath = GetShaderBinaryCachePath(m_shaders[0].m_name);
    m_shaders[0].m_type = type;
    m_shaders[0].m_debugMode = IsShaderDebugMode();

    m_name = m_shaders[0].m_name.substr(0, m_shaders[0].m_name.rfind('_'));

    if (Path::Exists(m_shaders[0].m_internalPath))
    {
        m_state = ResourceState::Loading;
    }
    else
    {
        m_state = ResourceState::Importing;
    }
}

ShaderResource::~ShaderResource()
{
    OnDestroy();
}

void ShaderResource::OnImport()
{
    SL_PROFILE;
    SL_LOG_TRACE("Loading shader \"{}\"", m_shaders[0].m_assetPath.data());
    m_shaders[0].m_source = FileIO::ReadString(m_shaders[0].m_assetPath.data());
    if (m_shaderCount == 2)
    {
        SL_LOG_TRACE("Loading shader \"{}\"", m_shaders[1].m_assetPath.c_str());
        m_shaders[1].m_source = FileIO::ReadString(m_shaders[1].m_assetPath.data());
    }

    if (m_shaders[0].m_source.empty() || (m_shaderCount == 2 && m_shaders[1].m_source.empty()))
    {
        SL_LOG_ERROR("No shader source {}", m_name.data());
        m_state = ResourceState::Destroying;
        return;
    }

    m_state = ResourceState::Building;
}

void ShaderResource::OnBuild()
{
    SL_PROFILE;
    SL_LOG_TRACE("Compiling SPIR-V {}", m_shaders[0].m_name.data());
    m_shaders[0].m_binary = ShaderCompiler::SourceToSpirv(m_shaders[0]);
    if (m_shaderCount == 2)
    {
        SL_LOG_TRACE("Compiling SPIR-V {}", m_shaders[1].m_name.data());
        m_shaders[1].m_binary = ShaderCompiler::SourceToSpirv(m_shaders[1]);
    }

    if (m_shaders[0].m_binary.empty() || (m_shaderCount == 2 && m_shaders[1].m_binary.empty()))
    {
        SL_LOG_ERROR("No shader binary {}", m_name.data());
        m_state = ResourceState::Destroying;
        return;
    }

    SL_LOG_TRACE("Writing SPIR-V file \"{}\"", m_shaders[0].m_internalPath);
    FileIO::WriteBinary<uint32_t>(m_shaders[0].m_internalPath.data(), m_shaders[0].m_binary);
    SL_LOG_TRACE("Writing SPIR-V file \"{}\"", m_shaders[1].m_internalPath);
    FileIO::WriteBinary<uint32_t>(m_shaders[1].m_internalPath.data(), m_shaders[1].m_binary);

    m_state = ResourceState::Uploading;
}

void ShaderResource::OnLoad()
{
    SL_PROFILE;

    SL_LOG_TRACE("Loading SPIR-V cache \"{}\"", m_shaders[0].m_internalPath.data());
    m_shaders[0].m_binary = FileIO::ReadBinary<uint32_t>(m_shaders[0].m_internalPath.data());
    if (m_shaderCount == 2)
    {
        SL_LOG_TRACE("Loading SPIR-V cache: \"{}\"", m_shaders[1].m_internalPath.data());
        m_shaders[1].m_binary = FileIO::ReadBinary<uint32_t>(m_shaders[1].m_internalPath.data());
    }

    if (m_shaders[0].m_binary.empty() || (m_shaderCount == 2 && m_shaders[1].m_binary.empty()))
    {
        SL_LOG_ERROR("No shader binary {}", m_name.data());
        m_state = ResourceState::Destroying;
        return;
    }

    m_state = ResourceState::Uploading;
}

void ShaderResource::OnUpload()
{
    SL_PROFILE;
    SL_LOG_TRACE("Uploading shader program {}", m_name.data());
    if (m_shaderCount == 2)
    {
        m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_binary, m_shaders[1].m_binary));
    }
    else
    {
        m_pShaderProgram.reset(Shader::Create(m_shaders[0].m_binary, m_shaders[0].m_type));
    }

    if (!m_pShaderProgram->GetHandle())
    {
        SL_LOG_ERROR("Failed to create shader program handle {}", m_name.data());
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
    m_shaders[0].m_binary.clear();
    std::vector<uint32_t>{}.swap(m_shaders[0].m_binary);

    if (m_shaderCount == 2)
    {
        m_shaders[1].m_source.clear();
        std::string{}.swap(m_shaders[1].m_source);
        m_shaders[1].m_binary.clear();
        std::vector<uint32_t>{}.swap(m_shaders[1].m_binary);
    }

    m_optimized = true;
}

} // namespace sl
