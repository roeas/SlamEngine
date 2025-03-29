#pragma once

#include "Core/Defines.h"

#include <glm/matrix.hpp>

#include <string>

namespace sl
{

class Shader
{
public:
    static Shader *Create(std::string_view vsSource, std::string_view fsSource);
    static Shader *Create(std::string_view shaderSource, ShaderType type);

public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void UploadUniform(int location, float value) = 0;
    virtual void UploadUniform(int location, const glm::vec2 &value) = 0;
    virtual void UploadUniform(int location, const glm::vec3 &value) = 0;
    virtual void UploadUniform(int location, const glm::vec4 &value) = 0;

    virtual void UploadUniform(int location, int32_t value) = 0;
    virtual void UploadUniform(int location, const glm::ivec2 &value) = 0;
    virtual void UploadUniform(int location, const glm::ivec3 &value) = 0;
    virtual void UploadUniform(int location, const glm::ivec4 &value) = 0;

    virtual void UploadUniform(int location, uint32_t value) = 0;
    virtual void UploadUniform(int location, const glm::uvec2 &value) = 0;
    virtual void UploadUniform(int location, const glm::uvec3 &value) = 0;
    virtual void UploadUniform(int location, const glm::uvec4 &value) = 0;

    virtual void UploadUniform(int location, const float *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::vec2 *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::vec3 *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::vec4 *pValue, size_t number) = 0;

    virtual void UploadUniform(int location, const int32_t *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::ivec2 *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::ivec3 *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::ivec4 *pValue, size_t number) = 0;

    virtual void UploadUniform(int location, const uint32_t *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::uvec2 *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::uvec3 *pValue, size_t number) = 0;
    virtual void UploadUniform(int location, const glm::uvec4 *pValue, size_t number) = 0;

    virtual void UploadUniform(int location, const glm::mat2 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat3 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat4 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat2x3 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat3x2 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat2x4 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat4x2 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat3x4 &value, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat4x3 &value, bool transpose = false) = 0;

    virtual void UploadUniform(int location, const glm::mat2 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat3 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat4 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat2x3 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat3x2 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat2x4 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat4x2 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat3x4 *pValue, size_t number, bool transpose = false) = 0;
    virtual void UploadUniform(int location, const glm::mat4x3 *pValue, size_t number, bool transpose = false) = 0;
};

} // namespace sl
