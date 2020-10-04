// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/threading/atomic_backoff_strategy.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
namespace
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
__forceinline void backoff_strategy_yield_impl() XR_NOEXCEPT
{
    ::SwitchToThread();
}

} // namespace anonymous

//-----------------------------------------------------------------------------------------------------------
/**
*/
void backoff_strategy_yield::operator()() const XR_NOEXCEPT
{
    backoff_strategy_yield_impl();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void backoff_strategy_pause::operator()() const XR_NOEXCEPT
{
    ::_mm_pause();
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
