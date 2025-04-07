#include "OpenGLShader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace sl
{

namespace
{

uint32_t UploadShader(const char *pSource, size_t size, ShaderType type)
{
    const GLint GLsize = (GLint)size;
    GLuint shaderHandle = glCreateShader(GLShaderType[(size_t)type]);

    glShaderSource(shaderHandle, 1, &pSource, &GLsize);
    glCompileShader(shaderHandle);

    GLint isCompiled = 0;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint logSize = 0;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logSize);

        // The logSize includes the NULL character
        std::vector<GLchar> infoLog(logSize);
        glGetShaderInfoLog(shaderHandle, logSize, nullptr, infoLog.data());
        SL_LOG_ERROR("Failed to compile shader: {}", infoLog.data());

        glDeleteShader(shaderHandle);
        return 0;
    }

    return shaderHandle;
}

uint32_t UploadProgram(uint32_t VSHandle, std::optional<uint32_t> optFSHandle = std::nullopt)
{
    uint32_t programHandle = glCreateProgram();

    glAttachShader(programHandle, VSHandle);
    if (optFSHandle)
    {
        glAttachShader(programHandle, optFSHandle.value());
    }
    glLinkProgram(programHandle);

    // Detach and delete shaders
    glDetachShader(programHandle, VSHandle);
    glDeleteShader(VSHandle);
    if (optFSHandle)
    {
        glDetachShader(programHandle, optFSHandle.value());
        glDeleteShader(optFSHandle.value());
    }

    GLint isLinked = 0;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint logSize = 0;
        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);

        // The logSize includes the NULL character
        std::vector<GLchar> infoLog(logSize);
        glGetProgramInfoLog(programHandle, logSize, nullptr, infoLog.data());
        SL_LOG_ERROR("Failed to link shader program: {}", infoLog.data());

        glDeleteProgram(programHandle);
        return 0;
    }

    return programHandle;
}

} // namespace

OpenGLShader::OpenGLShader(std::string_view vsSource, std::string_view fsSource)
{
    uint32_t VSHandle = UploadShader(vsSource.data(), vsSource.size(), ShaderType::VertexShader);
    uint32_t FSHandle = UploadShader(fsSource.data(), fsSource.size(), ShaderType::FragmentShader);
    m_programHandle = UploadProgram(VSHandle, FSHandle);
}

OpenGLShader::OpenGLShader(std::string_view shaderSource, ShaderType type)
{
    uint32_t shaderHandle = UploadShader(shaderSource.data(), shaderSource.size(), type);
    m_programHandle = UploadProgram(shaderHandle);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_programHandle);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_programHandle);
}

void OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

void OpenGLShader::UploadUniform(int location, float value) const
{
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniform(int location, const glm::vec2 &value) const
{
    glUniform2f(location, value.x, value.y);
}

void OpenGLShader::UploadUniform(int location, const glm::vec3 &value) const
{
    glUniform3f(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(int location, const glm::vec4 &value) const
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(int location, int32_t value) const
{
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec2 &value) const
{
    glUniform2i(location, value.x, value.y);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec3 &value) const
{
    glUniform3i(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec4 &value) const
{
    glUniform4i(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(int location, uint32_t value) const
{
    glUniform1ui(location, value);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec2 &value) const
{
    glUniform2ui(location, value.x, value.y);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec3 &value) const
{
    glUniform3ui(location, value.x, value.y, value.z);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec4 &value) const
{
    glUniform4ui(location, value.x, value.y, value.z, value.w);
}

void OpenGLShader::UploadUniform(int location, const float *pValue, size_t count) const
{
    glUniform1fv(location, (GLsizei)count, pValue);
}

void OpenGLShader::UploadUniform(int location, const glm::vec2 *pValue, size_t count) const
{
    glUniform2fv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::vec3 *pValue, size_t count) const
{
    glUniform3fv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::vec4 *pValue, size_t count) const
{
    glUniform4fv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const int32_t *pValue, size_t count) const
{
    glUniform1iv(location, (GLsizei)count, pValue);
}

void OpenGLShader::UploadUniform(int location, const glm::ivec2 *pValue, size_t count) const
{
    glUniform2iv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::ivec3 *pValue, size_t count) const
{
    glUniform3iv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::ivec4 *pValue, size_t count) const
{
    glUniform4iv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const uint32_t *pValue, size_t count) const
{
    glUniform1uiv(location, (GLsizei)count, pValue);
}

void OpenGLShader::UploadUniform(int location, const glm::uvec2 *pValue, size_t count) const
{
    glUniform2uiv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::uvec3 *pValue, size_t count) const
{
    glUniform3uiv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::uvec4 *pValue, size_t count) const
{
    glUniform4uiv(location, (GLsizei)count, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat2 &value, bool transpose) const
{
    glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3 &value, bool transpose) const
{
    glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4 &value, bool transpose) const
{
    glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat2x3 &value, bool transpose) const
{
    glUniformMatrix2x3fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3x2 &value, bool transpose) const
{
    glUniformMatrix3x2fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat2x4 &value, bool transpose) const
{
    glUniformMatrix2x4fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4x2 &value, bool transpose) const
{
    glUniformMatrix4x2fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3x4 &value, bool transpose) const
{
    glUniformMatrix3x4fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4x3 &value, bool transpose) const
{
    glUniformMatrix4x3fv(location, 1, transpose, glm::value_ptr(value));
}

void OpenGLShader::UploadUniform(int location, const glm::mat2 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix2fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix3fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix4fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat2x3 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix2x3fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3x2 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix3x2fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat2x4 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix2x4fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4x2 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix4x2fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat3x4 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix3x4fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

void OpenGLShader::UploadUniform(int location, const glm::mat4x3 *pValue, size_t count, bool transpose) const
{
    glUniformMatrix4x3fv(location, (GLsizei)count, transpose, glm::value_ptr(*pValue));
}

} // namespace sl

static_assert(std::is_same_v<GLchar, char>);
static_assert(std::is_same_v<GLuint, uint32_t>);
