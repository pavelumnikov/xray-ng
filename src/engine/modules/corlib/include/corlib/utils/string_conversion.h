// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/string_view.h"
#include "EASTL/optional.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
eastl::optional<int32_t> as_int(char const* str) XR_NOEXCEPT;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
inline eastl::optional<int32_t> as_int(string_view s) XR_NOEXCEPT
{
    return as_int(s.data());
}

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an Int, default if fail
*/
eastl::optional<int64_t> as_int64(string_view s) XR_NOEXCEPT;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an uint, default if fail
*/
eastl::optional<uint32_t> as_uint(string_view s) XR_NOEXCEPT;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as an uint64_t, default if fail
*/
eastl::optional<uint64_t> as_uint64(string_view s) XR_NOEXCEPT;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as a float, default if fail
*/
eastl::optional<float> as_float(string_view s) XR_NOEXCEPT;

//-----------------------------------------------------------------------------------------------------------
/** Attempts to return internal string as a double, default if fail
*/
eastl::optional<double> as_double(string_view s) XR_NOEXCEPT;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------