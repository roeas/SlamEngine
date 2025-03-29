#pragma once

#include "Render/RenderContext.h"

namespace sl
{

class OpenGLContext : public RenderContext
{
public:
    OpenGLContext(void *pWindow);
    ~OpenGLContext() override;

    void MakeCurrent() override;
    void SwapBuffers() override;

    void *GetWindow() override { return m_pWindow; }
    void *GetContext() override { return m_pContext; }

private:
    void *m_pWindow = nullptr;
    void *m_pContext = nullptr;
};

} // namespace sl
