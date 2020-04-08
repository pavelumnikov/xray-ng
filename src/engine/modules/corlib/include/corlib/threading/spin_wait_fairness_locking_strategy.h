// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_backoff_helpers.h"
#include "corlib/threading/atomic_backoff.h"
#include "corlib/threading/spin_wait_strategy_traits.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
// This particular locking scheme performs well  when lock contention is low, as 
// the while loop overhead is small and locks are acquired very quickly, but degrades 
// as many callers want the lock and most threads are doing a lot of interlocked 
// spinning. There are also no guarantees that a caller will ever acquire the lock
struct spin_wait_fairness_strategy final
    : spin_wait_strategy_traits<uint8_t, false>
{
    void reset(volatile locking_value& value) const noexcept;
    signalling_bool try_lock(volatile locking_value& value) const noexcept;
    void lock(volatile locking_value& value) noexcept;
    void unlock(volatile locking_value& value) const noexcept;
}; // struct spin_wait_fairness_strategy

//------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_fairness_strategy::reset(volatile locking_value& value) const noexcept
{
    value = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_fairness_strategy::lock(volatile locking_value& value) noexcept
{
    if(!this->try_lock(value))
    {
        backoff_do_while_function<default_atomic_backoff>([this, &value]()
        {
            return !this->try_lock(value);
        });
    }
}

//------------------------------------------------------------------------------
/**
*/
inline signalling_bool
spin_wait_fairness_strategy::try_lock(volatile locking_value& value) const noexcept
{
    return atomic_bcas<memory_order::sequential, locking_value>(value, 1, 0);
}

//------------------------------------------------------------------------------
/**
*/
inline void
spin_wait_fairness_strategy::unlock(volatile locking_value& value) const noexcept
{
    atomic_store<memory_order::release, locking_value>(value, 0);
}

} // namespace xr::threading
//------------------------------------------------------------------------------