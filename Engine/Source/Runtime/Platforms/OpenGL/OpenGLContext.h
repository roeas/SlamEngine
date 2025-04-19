#pragma once

#include "Renderer/GraphicsContext.h"

namespace sl
{

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(void *pWindow);
    OpenGLContext(const OpenGLContext &) = delete;
    OpenGLContext &operator=(const OpenGLContext &) = delete;
    OpenGLContext(OpenGLContext &&) = delete;
    OpenGLContext &operator=(OpenGLContext &&) = delete;
    ~OpenGLContext() override;

    void MakeCurrent() override;
    void SwapBuffers() override;
    void *GetContext() override { return m_pContext; }

private:
    void *m_pWindow = nullptr;
    void *m_pContext = nullptr;
};

} // namespace sl
