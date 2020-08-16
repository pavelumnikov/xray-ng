// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"
#include "corlib/threading/atomic_backoff_strategy.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

/// <summary>
/// Class that implements exponential backoff with selected strategies.
/// </summary>
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount = 16, size_t BackoffThreshold = 2 >
class atomic_backoff final
{
public:
    typedef SpinStrategy spin_strategy;
    typedef FailStrategy fail_strategy;

    /// <summary cref="atomic_backoff::atomic_backoff">
    /// </summary>
    XR_CONSTEXPR_CPP14_OR_INLINE atomic_backoff() XR_NOEXCEPT;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(atomic_backoff);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(atomic_backoff);

    /// <summary cref="atomic_backoff::pause">
    /// Pause for a while.
    /// </summary>
    void pause() XR_NOEXCEPT;

    /// <summary cref="atomic_backoff::bounded_pause">
    /// Pause for a few times and then return false immediately.
    /// </summary>
    signalling_bool bounded_pause() XR_NOEXCEPT;

    /// <summary cref="atomic_backoff::reset">
    /// Reset backoff counter.
    /// </summary>
    XR_CONSTEXPR_CPP14_OR_INLINE void reset() XR_NOEXCEPT;

private:
    /// My backoff counter
    size_t m_backoff_counter;

    /// Backoff strategy to do something before fail condition happened
    spin_strategy m_spin_strategy;

    /// Backoff strategy to do something after fail condition happened
    fail_strategy m_fail_strategy;
};

using default_atomic_backoff = atomic_backoff<backoff_strategy_pause, backoff_strategy_yield>;

template<size_t BackoffCount, size_t BackoffThreshold>
using default_atomic_backoff_timeout = atomic_backoff<backoff_strategy_pause, backoff_strategy_yield, BackoffCount, BackoffThreshold>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
XR_CONSTEXPR_CPP14_OR_INLINE
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::atomic_backoff() XR_NOEXCEPT
    : m_backoff_counter(1)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
void 
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::pause() XR_NOEXCEPT
{
    if(m_backoff_counter <= BackoffCount)
    {
        m_spin_strategy();
        // Pause twice as long the next time.
        m_backoff_counter *= BackoffThreshold;
    }
    else
    {
        // Pause is so long that we might as well yield CPU to scheduler.
        m_fail_strategy();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
signalling_bool
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::bounded_pause() XR_NOEXCEPT
{
    if(m_backoff_counter <= BackoffCount)
    {
        m_spin_strategy();
        // Pause twice as long the next time.
        m_backoff_counter *= BackoffThreshold;
        return true;
    }
    
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
XR_CONSTEXPR_CPP14_OR_INLINE void
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::reset() XR_NOEXCEPT
{
    atomic_store_relax<size_t>(m_backoff_counter, 1);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
