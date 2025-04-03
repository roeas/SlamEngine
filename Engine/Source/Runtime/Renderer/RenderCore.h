#pragma once

#include "Core/Defines.h"
#include "Renderer/RenderAPI.h"

#include <memory>

namespace sl
{

class Shader;
class VertexArray;

class RenderCore final
{
public:
    RenderCore() = delete;
    RenderCore(const RenderCore &) = delete;
    RenderCore &operator=(const RenderCore &) = delete;
    RenderCore(RenderCore &&) = delete;
    RenderCore &operator=(RenderCore &&) = delete;
    ~RenderCore() = delete;

    static void Init(GraphicsBackend backend);
    static GraphicsBackend GetBackend() { return m_backend; }

    static void SetClearColor(const glm::vec4 &color);
    static void SetClearDepth(float depth);
    static void SetClearStencil(int stencil);
    static void Clear();

    static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
    inline static GraphicsBackend m_backend = GraphicsBackend::None;
    inline static std::unique_ptr<RenderAPI> m_pRenderAPI;
};

} // namespace sl
