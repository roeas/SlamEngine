#pragma once

#include "Core/Defines.h"

namespace sl
{

struct ShaderInfo
{
    // Shader file name without extension
    std::string m_name;
    std::string m_assetPath;
    std::string m_binaryPath;
    std::string m_source;

    ShaderType m_type;
    bool m_debugMode;
};

} // namespace sl
