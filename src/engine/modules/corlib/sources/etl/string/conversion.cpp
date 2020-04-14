// This file is a part of xray-ng engine
//

#include "corlib/etl/string/conversion.h"
#include <stdlib.h>
#include <errno.h>

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::string
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
eastl::optional<int64_t>
as_int64(eastl::string_view s) noexcept
{
    auto const r = strtoll(eastl::data(s), nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<uint32_t>
as_uint(eastl::string_view s) noexcept
{
    auto const r = strtoul(eastl::data(s), nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<uint64_t>
as_uint64(eastl::string_view s) noexcept
{
    auto const r = strtoull(eastl::data(s), nullptr, 10);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<float>
as_float(eastl::string_view s) noexcept
{
    auto const r = strtof(eastl::data(s), nullptr);
    if(errno == ERANGE)
        return {};

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
eastl::optional<double>
as_double(eastl::string_view s) noexcept
{
    auto const r = strtod(eastl::data(s), nullptr);
    if(errno == ERANGE)
        return {};

    return r;
}

} // namespace xr::strings
//-----------------------------------------------------------------------------------------------------------
