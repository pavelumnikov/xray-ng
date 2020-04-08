// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/spin_wait_strategy_traits.h"
#include "corlib/signalling_bool.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

struct spin_wait_noop_strategy final : spin_wait_strategy_traits<uint8_t, false>
{
    void reset(volatile locking_value& locking_value) const noexcept;
    signalling_bool try_lock(volatile locking_value& locking_value) const noexcept;
    void lock(volatile locking_value& locking_value) const noexcept;
    void unlock(volatile locking_value& locking_value) const noexcept;
};

#pragma optimize( "", off )

//------------------------------------------------------------------------------
/**
*/
inline void 
spin_wait_noop_strategy::reset(volatile locking_value& locking_value) const noexcept
{
    XR_UNREFERENCED_PARAMETER(locking_value);
}

//------------------------------------------------------------------------------
/**
*/
inline signalling_bool
spin_wait_noop_strategy::try_lock(volatile locking_value& locking_value) const noexcept
{
    XR_UNREFERENCED_PARAMETER(locking_value);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_noop_strategy::lock(volatile locking_value& locking_value) const noexcept
{
    XR_UNREFERENCED_PARAMETER(locking_value);
}

//------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_noop_strategy::unlock(volatile locking_value& locking_value) const noexcept
{
    XR_UNREFERENCED_PARAMETER(locking_value);
}

#pragma optimize( "", on ) 

} // namespace xr::threading
//------------------------------------------------------------------------------