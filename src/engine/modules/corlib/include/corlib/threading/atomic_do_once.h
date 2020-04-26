// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_backoff.h"
#include "corlib/threading/atomic_backoff_helpers.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

struct do_once_state_enum
{
    enum list : uint8_t
    {
        //! No execution attempts have been undertaken yet
        uninitialized = 0,
        //! A thread is executing associated do-once routine
        pending,
        //! Do-once routine has been executed
        executed,
        //! Convenience alias
        initialization_complete = executed
    };
};
typedef do_once_state_enum::list do_once_state;
typedef volatile do_once_state atomic_do_once_state;

namespace details
{

//! Initialization from function that do not returns anything, e.g. initializes on demand
void run_initializer(void(*f)(), atomic_do_once_state& state);
//! Initialization from function that returns result and may need re-initialization on false result
void run_initializer(bool(*f)(), atomic_do_once_state& state);

} // namespace details

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename F >
void atomic_do_once(const F& func, atomic_do_once_state& state)
{
    typedef default_atomic_backoff do_once_backoff;

    // The loop in the implementation is necessary to avoid race when thread T2
    // that arrived in the middle of initialization attempt by another thread T1
    // has just made initialization possible.
    // In such a case T2 has to rely on T1 to initialize, but T1 may already be past
    // the point where it can recognize the changed conditions.
    while(threading::atomic_fetch_acq(state) != do_once_state::executed)
    {
        if(threading::atomic_fetch_acq(state) == do_once_state::uninitialized)
        {
            if(threading::atomic_bcas_seq(state, do_once_state::pending, do_once_state::uninitialized))
            {
                details::run_initializer(func, state);
                break;
            }
        }

        backoff_do_while_function<do_once_backoff>([&state]()
        {
            return threading::atomic_fetch_acq(state) == do_once_state::pending;
        });
    }
};

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
