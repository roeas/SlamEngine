#pragma once

#include "Resource/ShaderInfo.h"

#include <vector>

namespace sl
{

class ShaderCompiler final
{
public:
    ShaderCompiler() = delete;
    ShaderCompiler(const ShaderCompiler &) = delete;
    ShaderCompiler &operator=(const ShaderCompiler &) = delete;
    ShaderCompiler(ShaderCompiler &&) = delete;
    ShaderCompiler &operator=(ShaderCompiler &&) = delete;
    ~ShaderCompiler() = delete;

    static std::vector<uint32_t> SourceToSpirv(const ShaderInfo &info);
    static std::string SpirvToSource(std::vector<uint32_t> spirv);
    static void Reflect(const std::vector<uint32_t> &spirv);
};

} // namespace sl
