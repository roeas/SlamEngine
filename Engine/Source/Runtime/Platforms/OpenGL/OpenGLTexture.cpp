#include "OpenGLTexture.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLDefines.h"
#include "Renderer/RenderCore.h"

#include <glm/gtc/type_ptr.hpp>

namespace sl
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, TextureFormat format, bool genMipmap, uint32_t flags, const void *pData) :
    m_handle(0), m_width(width), m_height(height), m_flags(flags), m_format(format), m_genMipmap(genMipmap)
{
    Create(pData);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_handle);
}

void OpenGLTexture2D::SetBorderColor(const glm::vec4 &color) const
{
    glTextureParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
}

void OpenGLTexture2D::Clear(const void *pClearData) const
{
    glClearTexImage(m_handle, 0, GLTextureFormat[(size_t)m_format], GLDataType[(size_t)m_format], pClearData);
}

void OpenGLTexture2D::Resize(uint32_t width, uint32_t height, const void *pData)
{
    if (width <= 0 || height <= 0)
    {
        SL_LOG_ERROR("Invalid texture size ({}, {})", width, height);
        return;
    }

    if (m_width == width && m_height == height)
    {
        return;
    }

    glDeleteTextures(1, &m_handle);
    m_handle = 0;
    m_width = width;
    m_height = height;

    Create(pData);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_handle);
}

void OpenGLTexture2D::Create(const void *pData)
{
    if (m_handle)
    {
        return;
    }

    // Storage
    glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
    glTextureStorage2D(m_handle, 1, GLTextureInternalFormat[(size_t)m_format], m_width, m_height);

    // Data
    if (pData)
    {
        // TODO: Support uploading mipmap data
        glTextureSubImage2D(m_handle, 0, 0, 0, m_width, m_height, GLTextureFormat[(size_t)m_format], GLDataType[(size_t)m_format], pData);
    }

    // Wraps
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GLTextureWrap[(m_flags & SL_SAMPLER_U_MASK) >> SL_SAMPLER_U_SHIFT]);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GLTextureWrap[(m_flags & SL_SAMPLER_V_MASK) >> SL_SAMPLER_V_SHIFT]);

    // Filters
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureFilter[(m_flags & SL_SAMPLER_MIN_MASK) >> SL_SAMPLER_MIN_SHIFT]);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GLTextureFilter[(m_flags & SL_SAMPLER_MAG_MASK) >> SL_SAMPLER_MAG_SHIFT]);
    if (m_genMipmap)
    {
        glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureMipmapFilter[(m_flags & SL_SAMPLER_MIPMAP_MASK) >> SL_SAMPLER_MIPMAP_SHIFT]);
        glGenerateTextureMipmap(m_handle);
    }
}

OpenGLTextureCube::OpenGLTextureCube(uint32_t width, uint32_t height, uint32_t mipmapCount, TextureFormat format, bool genMipmap, uint32_t flags, const void *pData) :
    m_handle(0), m_width(width), m_height(height), m_mipmapCount(mipmapCount), m_flags(flags), m_format(format), m_genMipmap(genMipmap)
{
    Create(pData);
}

OpenGLTextureCube::~OpenGLTextureCube()
{

}

void OpenGLTextureCube::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_handle);
}

void OpenGLTextureCube::Create(const void *pData)
{
    if (m_handle)
    {
        return;
    }

    // Storage
    SL_ASSERT(m_width == m_height);
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_handle);
    glTextureStorage2D(m_handle, m_mipmapCount, GLTextureInternalFormat[(size_t)m_format], m_width, m_height);

    // Data
    if (pData)
    {
        // TMP
        constexpr size_t Mip0FaceSize = 256 * 256 * 4 * 32 / 8;
        unsigned char *pDataOffset = (unsigned char *)pData;
        for (size_t mip = 0; mip < m_mipmapCount; ++mip)
        {
            for (size_t face = 0; face < 6; ++face)
            {
                glTextureSubImage3D(m_handle, (GLint)mip, 0, 0, (GLint)face, m_width >> mip, m_height >> mip, 1,
                    GLTextureFormat[(size_t)m_format], GLDataType[(size_t)m_format], pDataOffset);
                pDataOffset += Mip0FaceSize >> (mip * 2);
            }
        }
    }

    // Wraps
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GLTextureWrap[(m_flags & SL_SAMPLER_U_MASK) >> SL_SAMPLER_U_SHIFT]);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GLTextureWrap[(m_flags & SL_SAMPLER_V_MASK) >> SL_SAMPLER_V_SHIFT]);
    glTextureParameteri(m_handle, GL_TEXTURE_WRAP_R, GLTextureWrap[(m_flags & SL_SAMPLER_W_MASK) >> SL_SAMPLER_W_SHIFT]);

    // Filters
    glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureFilter[(m_flags & SL_SAMPLER_MIN_MASK) >> SL_SAMPLER_MIN_SHIFT]);
    glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GLTextureFilter[(m_flags & SL_SAMPLER_MAG_MASK) >> SL_SAMPLER_MAG_SHIFT]);
    if (m_mipmapCount > 1 || m_genMipmap)
    {
        glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureMipmapFilter[(m_flags & SL_SAMPLER_MIPMAP_MASK) >> SL_SAMPLER_MIPMAP_SHIFT]);
    }

    if (m_genMipmap)
    {
        glGenerateTextureMipmap(m_handle);
    }
}

} // namespace sl
