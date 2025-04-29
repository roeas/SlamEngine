#pragma once

#include <nameof/nameof.hpp>

#include <sstream>

namespace nameof
{

template<typename... T>
inline constexpr std::string nameof_types()
{
    std::ostringstream oss;
    ((oss << (oss.tellp() == 0 ? "" : ", ") << nameof::nameof_type<T>()), ...);
    return oss.str();
}

} // namespace nameof
