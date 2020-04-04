/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

#include "corlib/threading/spin_wait_fairness_locking_strategy.h"
#include "corlib/threading/spin_wait_speculative_locking_strategy.h"
#include "corlib/threading/spin_wait_precise_locking_streategy.h"
#include "corlib/threading/spin_wait_noop_locking_strategy.h"
#include "corlib/etl/padded.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
namespace details
{

//------------------------------------------------------------------------------
template< typename LockingStrategy >
struct spin_wait_cache_line
{
    template< typename Strategy >
    struct cache_line_selector final
    {
        static constexpr size_t value = Strategy::cache_line_size;
    };

    struct default_cache_line_selector final
    {
        static constexpr size_t value = XR_MAX_CACHE_LINE_SIZE;
    };

    using locking_value = typename LockingStrategy::locking_value;

    using cache_line_selector_t = std::conditional_t<LockingStrategy::has_cache_line_size,
        cache_line_selector<LockingStrategy>, default_cache_line_selector>;

    static constexpr size_t cache_line_size = cache_line_selector_t::value;

    volatile locking_value m_lock;
    LockingStrategy m_locking_strategy;
};

} // namespace details
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// spin_wait provides spin-waiting functionality to (mostly) prevent kernel-level locking.
// Every locking strategy must be compatible with spin_wait only. 
// It must have these components:
// 1. try_lock method with returning boolean value as result of lock acquisition.
// 2. lock method that nothing returns and does actual locking.
// 3. unlock method that nothing returns and does unlocking.
// 4. has_cache_line_size static constant field that is needed by the cache line selector. 
// Default value is false.
//
// Maximum size of class/struct that produces strategy must be size of 
// spin_wait_cache_line::cache_line_size - sizeof(locking_value).
//
// In addition to (4): each strategy that has has_cache_line_size=true must provide static 
// constant expression field cache_line_size with some selected size of cache line.
template< typename LockingStrategy >
class spin_wait final 
    : protected etl::padded_with_cache_line_size<details::spin_wait_cache_line<LockingStrategy>>
{
public:
    spin_wait() noexcept;

    // Start spin waiting.
    void lock() noexcept;

    // Try acquiring lock (non-blocking) and return immediate.
    signalling_bool try_lock() noexcept;

    // Stop spin waiting.
    void unlock() noexcept;
}; // class spin_wait

using spin_wait_fairness = spin_wait<spin_wait_fairness_strategy>;
using spin_wait_speculative = spin_wait<spin_wait_speculative_strategy>;
using spin_wait_precise = spin_wait<spin_wait_precise_strategy>;
using spin_wait_noop = spin_wait<spin_wait_noop_strategy>;

//------------------------------------------------------------------------------
/**
*/
template< typename LockingStrategy >
spin_wait<LockingStrategy>::spin_wait() noexcept
{
    this->m_locking_strategy.reset(this->m_lock);
};

//------------------------------------------------------------------------------
/**
*/
template< typename LockingStrategy >
void spin_wait<LockingStrategy>::lock() noexcept
{
    this->m_locking_strategy.lock(this->m_lock);
}

//------------------------------------------------------------------------------
/**
*/
template< typename LockingStrategy >
signalling_bool spin_wait<LockingStrategy>::try_lock() noexcept
{
    return this->m_locking_strategy.try_lock(this->m_lock);
}

//------------------------------------------------------------------------------
/**
*/
template< typename LockingStrategy >
void spin_wait<LockingStrategy>::unlock() noexcept
{
    this->m_locking_strategy.unlock(this->m_lock);
}

} // namespace xr::threading
//------------------------------------------------------------------------------