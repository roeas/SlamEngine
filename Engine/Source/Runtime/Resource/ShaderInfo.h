#pragma once

#include "Core/Defines.h"

#include <cstdint>
#include <string>
#include <vector>

namespace sl
{

struct ShaderInfo
{
    // Shader file name without extension
    std::string m_name;
    std::string m_assetPath;
    std::string m_binaryPath;
    std::string m_source;
    std::vector<uint32_t> m_binary;

    ShaderType m_type;
    bool m_debugMode;
};

} // namespace sl
