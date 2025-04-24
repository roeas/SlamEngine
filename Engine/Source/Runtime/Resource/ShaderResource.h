#pragma once

#include "Core/Defines.h"
#include "Resource/Resource.h"
#include "Resource/ShaderInfo.h"

#include <memory>

namespace sl
{

class Shader;

class ShaderResource : public Resource
{
public:
    ShaderResource(std::string_view vsPath, std::string_view fsPath);
    ShaderResource(std::string_view path, ShaderType type);
    ShaderResource(const ShaderResource &) = delete;
    ShaderResource &operator=(const ShaderResource &) = delete;
    ShaderResource(ShaderResource &&) = delete;
    ShaderResource &operator=(ShaderResource &&) = delete;
    ~ShaderResource() override;

    void OnImport() override;
    void OnBuild() override;
    void OnLoad() override;
    void OnUpload() override;
    void OnReady() override;
    void OnDestroy() override;
    void DestroyCPUData() override;

    Shader *GetShaderProgram() const { return m_pShaderProgram.get(); }

    /*
     * We assume that a shader is named by its shader program name plus the stage suffix
     * Shader Program: XXX
     * Vertex Shader: XXX_vert.glsl
     * Fragment Shader: XXX_frag.glsl
     */
    std::string m_name;
    size_t m_shaderCount;
    ShaderInfo m_shaders[2];

    std::unique_ptr<Shader> m_pShaderProgram;
};

} // namespace sl
