#pragma once
#include <climits>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include "macros.h"

using byte  = uint8_t;
using sbyte = int8_t;

inline constexpr uint16_t KB = 1 << 10;
inline constexpr uint32_t MB = KB << 10;

inline constexpr size_t MAX_OSPATH = 256;

template <typename T>
class [[nodiscard]] ScopeExitRunner
{
public:
    explicit ScopeExitRunner(T func) : m_func(func)
    {
    }

    ~ScopeExitRunner()
    {
        m_func();
    }

    ScopeExitRunner(const ScopeExitRunner&)            = delete;
    ScopeExitRunner& operator=(const ScopeExitRunner&) = delete;

private:
    T m_func;
};

template <typename T>
ScopeExitRunner<T> makeScopeExitRunner(T&& func)
{
    return ScopeExitRunner<T>(std::forward<T>(func));
}

#define ON_SCOPE_EXIT(func) const auto CAT(scope_, __LINE__) = makeScopeExitRunner(func)

template <typename T>
constexpr T setBit(const T out, const byte offset, const bool value)
    requires(std::is_integral_v<T>)
{
    if (offset > sizeof(out) * CHAR_BIT - 1)
        return out;

    return static_cast<T>((out & ~(0b1 << offset)) | ((static_cast<T>(value) & 0b1) << offset));
}

template <typename T>
constexpr std::underlying_type_t<T> underlying(const T val)
{
    return static_cast<std::underlying_type_t<T>>(val);
}

constexpr byte lsn(const byte x)
{
    return x & 0x0F;
}

constexpr byte msn(const byte x)
{
    return x >> 4;
}

constexpr byte lsb(const uint16_t x)
{
    return x & 0xFF;
}

constexpr byte msb(const uint16_t x)
{
    return x >> 8;
}

constexpr uint16_t make16(const uint8_t lo, const uint8_t hi)
{
    return static_cast<uint16_t>((static_cast<uint16_t>(hi) << 8) | lo);
}

template <typename... T>
constexpr bool carryPerBit(const byte bit, const T... args)
{
    const uint64_t mask = (1ull << (bit + 1)) - 1;
    return ((static_cast<uint64_t>(args) & mask) + ...) > mask;
}

template <typename First, typename... T>
constexpr bool subCarryPerBit(const byte bit, const First first, const T... args)
{
    const uint64_t mask = (1ull << (bit + 1)) - 1;
    return ((static_cast<uint64_t>(args) & mask) + ...) > (first & mask);
}
