#pragma once

#include "Utils/Hash.hpp"

namespace sl
{

struct PostProcessingComponent
{
    void Reset()
    {

    }

    StringHashType m_meshResourceID = 0;
    StringHashType m_shaderResourceID = 0;
};

} // namespace sl
