#pragma once

#include "Utils/Hash.hpp"

namespace sl
{

struct RenderingComponent
{
    void Reset()
    {

    }

    StringHashType m_meshResourceID = 0;
    StringHashType m_shaderResourceID = 0;
    StringHashType m_entityIDShaderResourceID = 0;
    StringHashType m_materialResourceID = 0;
};

} // namespace sl
