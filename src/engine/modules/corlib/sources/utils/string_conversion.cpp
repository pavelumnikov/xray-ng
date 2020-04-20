// This file is a part of xray-ng engine
//

#include "corlib/utils/string_conversion.h"
#include <stdlib.h> // for str* functions
#include <errno.h> // for errno

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<int32_t> as_int(char const* str) noexcept
{
    auto const r = strtol(str, nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<int64_t> as_int64(string_view s) noexcept
{
    auto const r = strtoll(s.data(), nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<uint32_t> as_uint(string_view s) noexcept
{
    auto const r = strtoul(s.data(), nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<uint64_t> as_uint64(string_view s) noexcept
{
    auto const r = strtoull(s.data(), nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<float> as_float(string_view s) noexcept
{
    auto const r = strtof(s.data(), nullptr);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<double> as_double(string_view s) noexcept
{
    auto const r = strtod(s.data(), nullptr);
    if(errno == ERANGE)
        return {};

    return r;
}

} // namespace xr::strings
//-----------------------------------------------------------------------------------------------------------
