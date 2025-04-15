#pragma once

#include "Core/Core.h"

#include <nameof/nameof.hpp>

#include <optional>
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

template<auto... E>
inline constexpr std::string nameof_enums()
{
    std::ostringstream oss;
    ((oss << (oss.tellp() == 0 ? "" : ", ") << nameof::nameof_enum<E>()), ...);
    return oss.str();
}

template<typename E>
inline consteval auto enum_count() noexcept
{
    using D = std::decay_t<E>;
    static_assert(detail::nameof_enum_supported<D>::value, "nameof::nameof_enum unsupported compiler (https://github.com/Neargye/nameof#compiler-compatibility).");

    return detail::count_v<D>;
}

template<typename E>
inline constexpr std::optional<E> enumof_name(std::string_view name) noexcept
{
    using D = std::decay_t<E>;
    static_assert(detail::nameof_enum_supported<D>::value, "nameof::nameof_enum unsupported compiler (https://github.com/Neargye/nameof#compiler-compatibility).");
    static_assert(detail::count_v<D> > 0, "nameof::nameof_enum requires enum implementation and valid max and min.");

    for (std::size_t i = 0; i < detail::count_v<D>; ++i)
    {
        if (detail::names_v<D>[i] == name)
        {
            return detail::enum_value<D>(i);
        }
    }

    return std::nullopt;
}

} // namespace nameof
