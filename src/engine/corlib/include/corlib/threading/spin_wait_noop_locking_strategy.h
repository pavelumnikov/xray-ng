// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/spin_wait_strategy_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

struct spin_wait_noop_strategy final : spin_wait_strategy_traits<uint8_t, false>
{
    void reset(volatile locking_value& locking_value) const XR_NOEXCEPT;
    signalling_bool try_lock(volatile locking_value& locking_value) const XR_NOEXCEPT;
    void lock(volatile locking_value& locking_value) const XR_NOEXCEPT;
    void unlock(volatile locking_value& locking_value) const XR_NOEXCEPT;
};

#pragma optimize( "", off )

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void 
spin_wait_noop_strategy::reset(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(locking_value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline signalling_bool
spin_wait_noop_strategy::try_lock(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(locking_value);
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_noop_strategy::lock(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(locking_value);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_noop_strategy::unlock(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(locking_value);
}

#pragma optimize( "", on ) 

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
