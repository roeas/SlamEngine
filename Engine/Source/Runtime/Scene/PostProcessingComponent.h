#pragma once

#include "Shader/Shared.h"
#include "Utils/Hash.hpp"

namespace sl
{

enum class ToneMappingMode : uint8_t
{
    Clamp       = SL_TYPE_TONEMAPPING_CLAMP,
    Reinhard    = SL_TYPE_TONEMAPPING_REINHARD,
    HableFilmic = SL_TYPE_TONEMAPPING_HABLEFILMIC,
    ACES        = SL_TYPE_TONEMAPPING_ACES,
    ACES_Simple = SL_TYPE_TONEMAPPING_ACES_SIMPLE,
    PBRNeutral  = SL_TYPE_TONEMAPPING_PBRNEUTRAL,
};

struct PostProcessingComponent
{
    void Reset()
    {

    }

    ToneMappingMode m_toneMappingMode = ToneMappingMode::PBRNeutral;

    StringHashType m_meshResourceID = 0;
    StringHashType m_shaderResourceID = 0;
};

} // namespace sl
