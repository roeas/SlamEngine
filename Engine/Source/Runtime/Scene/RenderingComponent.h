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
    Shader *m_pShader = nullptr;
    Shader *m_pIDShader = nullptr;

    StringHashType m_meshResourceID = 0;
    StringHashType m_textureResourceID = 0;
};

} // namespace sl
