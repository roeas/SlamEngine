#pragma once

#include "Core/Defines.h"

#include <glm/matrix.hpp>

#include <string>
#include <span>

namespace sl
{

class Shader
{
public:
    static Shader *Create(std::string_view vsSource, std::string_view fsSource);
    static Shader *Create(std::string_view shaderSource, ShaderType type);

    static Shader *Create(std::span<const uint32_t> vsBinary, std::span<const uint32_t> fsBinary);
    static Shader *Create(std::span<const uint32_t> binary, ShaderType type);

public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetHandle() const = 0;

    virtual void UploadUniform(int location, float value) const = 0;
    virtual void UploadUniform(int location, const glm::vec2 &value) const = 0;
    virtual void UploadUniform(int location, const glm::vec3 &value) const = 0;
    virtual void UploadUniform(int location, const glm::vec4 &value) const = 0;

    virtual void UploadUniform(int location, int32_t value) const = 0;
    virtual void UploadUniform(int location, const glm::ivec2 &value) const = 0;
    virtual void UploadUniform(int location, const glm::ivec3 &value) const = 0;
    virtual void UploadUniform(int location, const glm::ivec4 &value) const = 0;

    virtual void UploadUniform(int location, uint32_t value) const = 0;
    virtual void UploadUniform(int location, const glm::uvec2 &value) const = 0;
    virtual void UploadUniform(int location, const glm::uvec3 &value) const = 0;
    virtual void UploadUniform(int location, const glm::uvec4 &value) const = 0;

    virtual void UploadUniform(int location, const float *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::vec2 *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::vec3 *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::vec4 *pValue, size_t count) const = 0;

    virtual void UploadUniform(int location, const int32_t *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::ivec2 *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::ivec3 *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::ivec4 *pValue, size_t count) const = 0;

    virtual void UploadUniform(int location, const uint32_t *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::uvec2 *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::uvec3 *pValue, size_t count) const = 0;
    virtual void UploadUniform(int location, const glm::uvec4 *pValue, size_t count) const = 0;

    virtual void UploadUniform(int location, const glm::mat2 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat3 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat4 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat2x3 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat3x2 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat2x4 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat4x2 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat3x4 &value, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat4x3 &value, bool transpose = false) const = 0;

    virtual void UploadUniform(int location, const glm::mat2 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat3 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat4 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat2x3 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat3x2 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat2x4 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat4x2 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat3x4 *pValue, size_t count, bool transpose = false) const = 0;
    virtual void UploadUniform(int location, const glm::mat4x3 *pValue, size_t count, bool transpose = false) const = 0;
};

} // namespace sl
