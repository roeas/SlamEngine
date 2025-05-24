#pragma once

#include "Utils/Hash.hpp"

namespace sl
{

struct SkyComponent
{
    void Reset()
    {
        m_factor = 1.0f;
    }

    float m_factor = 1.0f;
    StringHashType m_meshResourceID = 0;
    StringHashType m_shaderResourceID = 0;
    StringHashType m_textureResourceID = 0;
};

} // namespace sl
