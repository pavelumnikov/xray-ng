// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/namespaces.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, details)

#if XR_GCC_COMPILER_FAMILY
static inline bool (likely)(bool x)
{
    return __builtin_expect((x), true);
}
static inline bool (unlikely)(bool x)
{
    return __builtin_expect((x), false);
}
#else
static inline bool (likely)(bool x)
{
    return x;
}
static inline bool (unlikely)(bool x)
{
    return x;
}
#endif

XR_NAMESPACE_END(xr, details)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------

// XR_UNLIKELY macro
#if defined(XR_UNLIKELY)
#   error please do not define XR_UNLIKELY macros
#endif // #if defined(XR_LIKELY)

#define XR_UNLIKELY(x) xr::details::unlikely((x))

//-----------------------------------------------------------------------------------------------------------

// XR_LIKELY macro
#if defined(XR_LIKELY)
#   error please do not define XR_LIKELY macros
#endif // #if defined(XR_LIKELY)

#define XR_LIKELY(x) xr::details::likely((x))

//-----------------------------------------------------------------------------------------------------------