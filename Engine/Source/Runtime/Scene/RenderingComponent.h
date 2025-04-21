#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

namespace sl
{

struct RenderingComponent
{
    void Reset()
    {

    }

    // TMP
    VertexArray *m_pVertexArray = nullptr;
    Texture2D *m_pTexture = nullptr;
    Shader *m_pShader = nullptr;
    Shader *m_pIDShader = nullptr;
};

} // namespace sl
