#pragma once

// std
#include <functional>

namespace Core::Utils
{

template <typename T, typename... Rest> void hashCombine(std::size_t &seed, const T &v, Rest &...rest)
{
    seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

} // namespace Core::Utils
