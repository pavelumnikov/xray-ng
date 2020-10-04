// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

struct backoff_strategy_nothing final
{
    void operator()() const XR_NOEXCEPT;
};

#if defined(XRAY_PLATFORM_64BIT)
inline void backoff_strategy_nothing::operator()() const XR_NOEXCEPT
{
    (void)0;
}
#endif // defined(XRAY_PLATFORM_64BIT)


struct backoff_strategy_yield final
{
    void operator()() const XR_NOEXCEPT;
};

struct backoff_strategy_pause final
{
    void operator()() const XR_NOEXCEPT;
};

struct backoff_strategy_noop final
{
    void operator()() const XR_NOEXCEPT;
};

#if defined(XRAY_PLATFORM_64BIT)
inline void backoff_strategy_noop::operator()() const XR_NOEXCEPT
{
    (void)0;
}
#endif // defined(XRAY_PLATFORM_64BIT)

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
