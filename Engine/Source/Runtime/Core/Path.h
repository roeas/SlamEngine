#pragma once

#include "Core/Core.h"

#include <filesystem>

namespace sl
{

class Path final
{
public:
    Path() = delete;
    Path(const Path &) = delete;
    Path &operator=(const Path &) = delete;
    Path(Path &&) = delete;
    Path &operator=(Path &&) = delete;
    ~Path() = delete;

    SL_FORCEINLINE static bool Exists(std::string_view path)
    {
        return std::filesystem::exists(path);
    }

    SL_FORCEINLINE static std::string NameWithExtension(std::string_view path)
    {
        return std::filesystem::path{ path }.filename().generic_string();
    }

    SL_FORCEINLINE static std::string NameWithoutExtension(std::string_view path)
    {
        return std::filesystem::path{ path }.stem().generic_string();
    }

    SL_FORCEINLINE static std::string Extension(std::string_view path)
    {
        return std::filesystem::path{ path }.extension().generic_string();
    }

    SL_FORCEINLINE static std::string Parent(std::string_view path)
    {
        return std::filesystem::path{ path }.parent_path().generic_string();
    }

    SL_FORCEINLINE static bool Contain(std::string_view base, std::string_view sub)
    {
        auto rel = std::filesystem::relative(sub, base);
        return !rel.empty() && *(rel.c_str()) != '.';
    }

    SL_FORCEINLINE static std::string GetRoot()
    {
        return SL_ROOT_PATH;
    }

    SL_FORCEINLINE static std::string GetAsset()
    {
        return SL_ASSET_PATH;
    }

    SL_FORCEINLINE static std::string FromeRoot(std::string_view path = "")
    {
        std::filesystem::path result{ SL_ROOT_PATH };
        result /= path;
        return result.generic_string();
    }

    SL_FORCEINLINE static std::string FromeAsset(std::string_view path = "")
    {
        std::filesystem::path result{ SL_ASSET_PATH };
        result /= path;
        return result.generic_string();
    }

    SL_FORCEINLINE static std::string Join(auto &&...args)
    {
        std::filesystem::path result;
        (result /= ... /= std::filesystem::path{ std::forward<decltype(args)>(args) });
        return result.generic_string();
    }
};

} // namespace sl
