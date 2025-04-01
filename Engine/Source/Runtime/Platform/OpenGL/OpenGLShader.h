#pragma once

#include "Render/Shader.h"

namespace sl
{

class OpenGLShader : public Shader
{
public:
    OpenGLShader(std::string_view vsSource, std::string_view fsSource);
    OpenGLShader(std::string_view shaderSource, ShaderType type);
    ~OpenGLShader() override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t GetHandle() const override { return m_programHandle; }

    void UploadUniform(int location, float value) const override;
    void UploadUniform(int location, const glm::vec2 &value) const override;
    void UploadUniform(int location, const glm::vec3 &value) const override;
    void UploadUniform(int location, const glm::vec4 &value) const override;

    void UploadUniform(int location, int32_t value) const override;
    void UploadUniform(int location, const glm::i32vec2 &value) const override;
    void UploadUniform(int location, const glm::i32vec3 &value) const override;
    void UploadUniform(int location, const glm::i32vec4 &value) const override;

    void UploadUniform(int location, uint32_t value) const override;
    void UploadUniform(int location, const glm::u32vec2 &value) const override;
    void UploadUniform(int location, const glm::u32vec3 &value) const override;
    void UploadUniform(int location, const glm::u32vec4 &value) const override;

    void UploadUniform(int location, const float *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::vec2 *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::vec3 *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::vec4 *pValue, size_t count) const override;

    void UploadUniform(int location, const int32_t *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::i32vec2 *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::i32vec3 *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::i32vec4 *pValue, size_t count) const override;

    void UploadUniform(int location, const uint32_t *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::u32vec2 *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::u32vec3 *pValue, size_t count) const override;
    void UploadUniform(int location, const glm::u32vec4 *pValue, size_t count) const override;

    void UploadUniform(int location, const glm::mat2 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat3 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat4 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat2x3 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat3x2 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat2x4 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat4x2 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat3x4 &value, bool transpose) const override;
    void UploadUniform(int location, const glm::mat4x3 &value, bool transpose) const override;

    void UploadUniform(int location, const glm::mat2 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat3 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat4 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat2x3 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat3x2 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat2x4 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat4x2 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat3x4 *pValue, size_t count, bool transpose) const override;
    void UploadUniform(int location, const glm::mat4x3 *pValue, size_t count, bool transpose) const override;

private:
    uint32_t m_programHandle = 0;
};

} // namespace sl
