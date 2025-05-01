#pragma once

#include <concepts>
#include <cstdint>
#include <string_view>

namespace sl
{

template<class T>
concept Fnv1aType = std::same_as<T, uint32_t> || std::same_as<T, uint64_t>;

template<Fnv1aType T>
struct Fnv1aConfig;

template<>
struct Fnv1aConfig<uint32_t>
{
    static constexpr uint32_t offset = 0x811c9dc5;
    static constexpr uint32_t prime  = 0x01000193;
};

template<>
struct Fnv1aConfig<uint64_t>
{
    static constexpr uint64_t offset = 0xcbf29ce484222325;
    static constexpr uint64_t prime  = 0x00000100000001b3;
};

using StringHashType = uint64_t;

template<Fnv1aType T = StringHashType>
constexpr T StringHash(std::string_view str)
{
    T hash = Fnv1aConfig<T>::offset;
    for (auto c : str)
    {
        hash ^= static_cast<T>(c);
        hash *= Fnv1aConfig<T>::prime;
    }

    return hash;
}

} // namespace sl
