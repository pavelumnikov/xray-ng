// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_backoff.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

// Fast semaphore object that only tracks count of threads that could access 
// some critical section, but it's vital that programmer knows what he does: 
// semaphore only tracks count of threads and "blocks" when thread count is
// higher than MaxThreads, and unblocks other case.
template<size_t MaxThreads, typename BackoffType = default_atomic_backoff>
class fast_semaphore
{
public:
    constexpr fast_semaphore() XR_NOEXCEPT;

    // Wait for semaphore object monitor.
    signalling_bool wait() XR_NOEXCEPT;

    // Signal semaphore object to stop fencing.
    void signal() XR_NOEXCEPT;

private:
    auto constexpr c_max_simultaneous_accessing_threads = MaxThreads;
    atomic_size_t m_active;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< size_t MaxThreads, typename BackoffType >
constexpr
fast_semaphore<MaxThreads, BackoffType>::fast_semaphore() XR_NOEXCEPT
    : m_active(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< size_t MaxThreads, typename BackoffType >
inline signalling_bool
fast_semaphore<MaxThreads, BackoffType>::wait() XR_NOEXCEPT
{
    bool rescan_bins = false;

    for(;;)
    {
        size_t const prev_accessing_count = atomic_fetch_acq(m_active);

        if(prev_accessing_count < c_max_simultaneous_accessing_threads)
        {
            size_t const next_thread_index = (prev_accessing_count + 1);
            size_t const accessing_count = atomic_cas_seq(
                m_active, next_thread_index, prev_accessing_count);

            if(accessing_count == prev_accessing_count)
                break;
        }
        else
        {
            BackoffType backoff;
            do
            {
                backoff.pause();
            } while(m_active == prev_accessing_count);

            rescan_bins = true;
            break;
        }
    }

    return rescan_bins;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< size_t MaxThreads, typename BackoffType >
inline void
fast_semaphore<MaxThreads, BackoffType>::signal() XR_NOEXCEPT
{
    (void)atomic_fetch_sub_seq(m_active, 1);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
