// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/string_view.h"
#include "EASTL/optional.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::string
{
//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
eastl::optional<int32_t> as_int(char const* str) noexcept;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
inline eastl::optional<int32_t> as_int(eastl::string_view s) noexcept
{
    return as_int(s.data());
}

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
eastl::optional<int64_t> as_int64(eastl::string_view s) noexcept;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an uint, default if fail
*/
eastl::optional<uint32_t> as_uint(eastl::string_view s) noexcept;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an uint64_t, default if fail
*/
eastl::optional<uint64_t> as_uint64(eastl::string_view s) noexcept;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as a float, default if fail
*/
eastl::optional<float> as_float(eastl::string_view s) noexcept;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as a double, default if fail
*/
eastl::optional<double> as_double(eastl::string_view s) noexcept;

} // namespace xr::etl::string
//-----------------------------------------------------------------------------------------------------------