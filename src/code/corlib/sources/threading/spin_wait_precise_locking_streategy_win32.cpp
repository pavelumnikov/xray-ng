// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/threading/spin_wait_precise_locking_streategy.h"
#include "corlib/threading/interlocked.h"
#include "corlib/sys/thread.h"
#include "../os_include_win32.h"
#include <cassert>

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

static constexpr uint32_t yield_iteration = 30; // yeild after 30 iterations
static constexpr uint32_t max_sleep_iteration = 40;
static constexpr uint32_t seed_value = 100;

//-----------------------------------------------------------------------------------------------------------
/**
*/
void 
spin_wait_precise_strategy::reset(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    locking_value.dest = 0;
    locking_value.exchange = seed_value;
    locking_value.compare = 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
signalling_bool
spin_wait_precise_strategy::try_lock(volatile locking_value& lockingValue) const XR_NOEXCEPT
{
    return atomic_cas_seq(
        lockingValue.dest, lockingValue.exchange, lockingValue.compare) == 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void 
spin_wait_precise_strategy::lock(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    uint32_t iterations { 0 };

    while(true)
    {
        // A thread alreading owning the lock shouldn't be allowed to wait to 
        // acquire the lock - reentrant safe
        if(locking_value.dest == ::GetCurrentThreadId())
            break;

        // Spinning in a loop of interlockedxxx calls can reduce the available 
        // memory bandwidth and slow down the rest of the system. Interlocked 
        // calls are expensive in their use of the system memory bus. It is 
        // better to see if the 'dest' value is what it is expected and then 
        // retry interlockedxx.

        if(atomic_cas_seq(
            locking_value.dest, locking_value.exchange, locking_value.compare) == 0)
        {
            //assign CurrentThreadId to dest to make it re-entrant safe
            locking_value.dest = ::GetCurrentThreadId();
            // lock acquired
            break;
        }

        // spin wait to acquire 
        while(locking_value.dest != locking_value.compare)
        {
            if(iterations >= yield_iteration)
            {
                if(iterations + yield_iteration >= max_sleep_iteration)
                    sys::yield();

                if((iterations >= yield_iteration) &&
                    (iterations < max_sleep_iteration))
                {
                    iterations = 0;
                    ::SwitchToThread();
                }
            }

            // yield processor on multi-processor but if on single processor 
            // then give other thread the CPU
            iterations++;
            ::_mm_pause();
        }
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void 
spin_wait_precise_strategy::unlock(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    assert((locking_value.dest == GetCurrentThreadId()) && 
        "Unexpected thread-id in release");

    uint32_t const thread_id = ::GetCurrentThreadId();
    (void)atomic_cas_seq(locking_value.dest, locking_value.compare, thread_id);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
