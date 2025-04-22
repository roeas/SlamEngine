#pragma once

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Utils/Hash.hpp"

namespace sl
{

struct RenderingComponent
{
    void Reset()
    {

    }

    // TMP
    VertexArray *m_pVertexArray = nullptr;
    Shader *m_pShader = nullptr;
    Shader *m_pIDShader = nullptr;

    StringHashType m_textureResourceID = 0;
};

} // namespace sl
