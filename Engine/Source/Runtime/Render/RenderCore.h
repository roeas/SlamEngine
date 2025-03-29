#pragma once

#include "Core/Defines.h"

namespace sl
{

class RenderCore final
{
public:
    RenderCore() = delete;
    RenderCore(const RenderCore &) = delete;
    RenderCore &operator=(const RenderCore &) = delete;
    RenderCore(RenderCore &&) = delete;
    RenderCore &operator=(RenderCore &&) = delete;
    ~RenderCore() = delete;

    static void Init();

    static void SetBackend(GraphicsBackend backend) { m_backend = backend; }
    static GraphicsBackend GetBackend() { return m_backend; }

private:
    inline static GraphicsBackend m_backend = GraphicsBackend::None;
};

} // namespace sl
