// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"
#include "corlib/threading/atomic_backoff_strategy.h"
#include "corlib/signalling_bool.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::threading
{

/// <summary>
/// Class that implements exponential backoff with selected strategies.
/// </summary>
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount = 16, size_t BackoffThreshold = 2 >
class atomic_backoff final
{
public:
    using spin_strategy = SpinStrategy;
    using fail_strategy = FailStrategy;

    /// <summary cref="atomic_backoff::atomic_backoff">
    /// </summary>
    constexpr atomic_backoff() noexcept;

    /// <summary cref="atomic_backoff::atomic_backoff">
    /// </summary>
    constexpr atomic_backoff(atomic_backoff&& mov) noexcept;

    /// <summary cref="atomic_backoff::atomic_backoff">
    /// </summary>
    atomic_backoff& operator=(atomic_backoff&& mov) noexcept;

    atomic_backoff(const atomic_backoff&) = delete;
    atomic_backoff& operator=(const atomic_backoff&) = delete;

    /// <summary cref="atomic_backoff::pause">
    /// Pause for a while.
    /// </summary>
    void pause() noexcept;

    /// <summary cref="atomic_backoff::bounded_pause">
    /// Pause for a few times and then return false immediately.
    /// </summary>
    signalling_bool bounded_pause() noexcept;

    /// <summary cref="atomic_backoff::reset">
    /// Reset backoff counter.
    /// </summary>
    constexpr void reset() noexcept;

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
constexpr
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::atomic_backoff() noexcept
    : m_backoff_counter(1)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
constexpr 
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::atomic_backoff(atomic_backoff&& mov) noexcept
    : m_backoff_counter(move(mov.m_backoff_counter))
    , m_spin_strategy(move(mov.m_spin_strategy))
    , m_fail_strategy(move(mov.m_fail_strategy))
{
    mov.m_backoff_counter = 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >&
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::operator=(atomic_backoff&& mov) noexcept
{
    m_backoff_counter = move(mov.m_backoff_counter);
    mov.m_backoff_counter = 0;

    m_spin_strategy = move(mov.m_spin_strategy);
    m_fail_strategy = move(mov.m_fail_strategy);

    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename SpinStrategy, typename FailStrategy, size_t BackoffCount, size_t BackoffThreshold >
void 
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::pause() noexcept
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
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::bounded_pause() noexcept
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
constexpr void
atomic_backoff< SpinStrategy, FailStrategy, BackoffCount, BackoffThreshold >::reset() noexcept
{
    atomic_store_relax<size_t>(m_backoff_counter, 1);
}

} // namespace xr::threading
//-----------------------------------------------------------------------------------------------------------
