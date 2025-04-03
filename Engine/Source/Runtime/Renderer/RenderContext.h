#pragma once

namespace sl
{

class RenderContext
{
public:
    static RenderContext *Create(void *pWindow);

public:
    virtual ~RenderContext() = default;

    virtual void MakeCurrent() = 0;
    virtual void SwapBuffers() = 0;

    virtual void *GetWindow() = 0;
    virtual void *GetContext() = 0;
};

} // namespace sl
