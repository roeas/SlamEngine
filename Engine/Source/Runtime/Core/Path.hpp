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

    SL_FORCEINLINE static std::string FromeRoot(std::string_view path = "")
    {
        return (std::filesystem::path{ SL_ROOT_PATH } / path).generic_string();
    }

    SL_FORCEINLINE static std::string FromeAsset(std::string_view path = "")
    {
        return (std::filesystem::path{ SL_ASSET_PATH } / path).generic_string();
    }

    SL_FORCEINLINE static std::string Join(auto &&...args)
    {
        return (... / std::filesystem::path{ std::forward<decltype(args)>(args) }).generic_string();
    }
};

} // namespace sl
