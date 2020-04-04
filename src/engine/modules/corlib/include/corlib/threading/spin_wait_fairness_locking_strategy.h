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