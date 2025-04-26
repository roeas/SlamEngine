#include "OpenGLFramebuffer.h"

#include "Core/Log.h"
#include "Platforms/OpenGL/OpenGLDefines.h"
#include "Renderer/RenderCore.h"
#include "Renderer/Texture.h"

namespace sl
{

OpenGLFramebuffer::OpenGLFramebuffer(std::initializer_list<Texture2D *> pTextures, bool destroy) :
    m_handle(0), m_width(0), m_height(0), m_colorAttachmentCount(0), m_destroyTextures(destroy)
{
    size_t textureCount = pTextures.size();
    if (textureCount == 0)
    {
        SL_LOG_ERROR("Cannot create framebuffer without any attachment!");
        return;
    }

    uint32_t minWidth = (*pTextures.begin())->GetWidth();
    uint32_t minHeight = (*pTextures.begin())->GetHeight();
    uint32_t colorAttachmentIndex = 0;
    m_attachments.reserve(textureCount);

    for (auto *pTexture : pTextures)
    {
        AttachmentType type = TextureFormatToAttachmentType[(size_t)pTexture->GetFormat()];
        GLenum attachmentPoint = GLAttachmentPoint[(size_t)type];

        if (type == AttachmentType::Color)
        {
            // point = GL_COLOR_ATTACHMENT0 + index
            attachmentPoint += colorAttachmentIndex++;
        }
        m_attachments.emplace_back(pTexture, attachmentPoint);

        uint32_t textureWidth = pTexture->GetWidth();
        uint32_t textureHeight = pTexture->GetHeight();
        if (minWidth != textureWidth || minHeight != textureHeight)
        {
            minWidth = std::min(minWidth, textureWidth);
            minHeight = std::min(minHeight, textureHeight);
            SL_LOG_WARN("Creating framebuffer with textures of different sizes, shrink to the minimal one: ({}, {})", minWidth, minHeight);
        }
    }

    m_width = minWidth;
    m_height = minHeight;
    m_colorAttachmentCount = colorAttachmentIndex;

    Create();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    if (m_destroyTextures)
    {
        for (auto &attachment : m_attachments)
        {
            delete attachment.m_pTexture;
        }
    }
    
    glDeleteFramebuffers(1, &m_handle);
}

void OpenGLFramebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glViewport(0, 0, m_width, m_height);
}

void OpenGLFramebuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width < 1 || height < 1)
    {
        SL_LOG_ERROR("Invalid frame bufferr size: ({}, {})", width, height);
        return;
    }
    if (m_width == width && m_height == height)
    {
        return;
    }

    m_width = width;
    m_height = height;
    for (auto &attachment : m_attachments)
    {
        attachment.m_pTexture->Resize(m_width, m_height);
    }

    glDeleteFramebuffers(1, &m_handle);
    m_handle = 0;

    Create();
}

void OpenGLFramebuffer::Clear(uint32_t attachmentIndex, const void *pClearData) const
{
    if (attachmentIndex >= m_attachments.size())
    {
        SL_LOG_ERROR("Attachment index out of range!");
        return;
    }

    m_attachments[attachmentIndex].m_pTexture->Clear(pClearData);
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
{
    if (attachmentIndex >= m_attachments.size())
    {
        SL_LOG_ERROR("Attachment index out of range!");
        return 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

    int data;
    TextureFormat format = m_attachments.at(attachmentIndex).m_pTexture->GetFormat();
    // We assume that origin of texture is on the upper left,
    // but the origin of OpenGL texture is on the lower left.
    glReadPixels(x, m_height - y, 1, 1, GLTextureFormat[(size_t)format], GLDataType[(size_t)format], &data);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return data;
}

uint32_t OpenGLFramebuffer::GetAttachmentHandle(size_t attachmentIndex) const
{
    if (attachmentIndex >= m_attachments.size())
    {
        SL_LOG_ERROR("Attachment index out of range!");
        return 0;
    }

    return m_attachments[attachmentIndex].m_pTexture->GetHandle();
}

void OpenGLFramebuffer::Create()
{
    if (m_handle)
    {
        return;
    }

    glGenFramebuffers(1, &m_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    for (const auto &attachment : m_attachments)
    {
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.m_point, GL_TEXTURE_2D, attachment.m_pTexture->GetHandle(), 0);
    }

    // MRT
    if (m_colorAttachmentCount > 1)
    {
        std::vector<GLenum> buffers;
        buffers.reserve(m_colorAttachmentCount);
        for (uint32_t i = 0; i < m_colorAttachmentCount; ++i)
        {
            buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
        }

        glNamedFramebufferDrawBuffers(m_handle, m_colorAttachmentCount, buffers.data());
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        SL_LOG_ERROR("Incomplete frame buffer!");
        glDeleteFramebuffers(1, &m_handle);
        m_handle = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace sl
