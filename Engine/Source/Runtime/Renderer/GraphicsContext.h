#pragma once

namespace sl
{

class GraphicsContext
{
public:
    static GraphicsContext *Create(void *pWindow);

public:
    virtual ~GraphicsContext() = default;

    virtual void MakeCurrent() = 0;
    virtual void SwapBuffers() = 0;
    virtual void *GetContext() = 0;
};

} // namespace sl
