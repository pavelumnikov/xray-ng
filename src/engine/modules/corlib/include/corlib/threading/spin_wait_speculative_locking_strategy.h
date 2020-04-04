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

#include "corlib/threading/atomic_backoff.h"
#include "corlib/threading/spin_wait_strategy_traits.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

/// <summary>
/// On platforms with proper HW support, this lock may speculatively execute its critical sections, using HW mechanisms
/// to detect real data races and ensure atomicity of the critical sections. In particular, it uses 
/// Intel(R) Transactional Synchronization Extensions(Intel(R) TSX). Without such HW support, it behaves like a 
/// <c>spin_wait_fairness_strategy</c> strategy. It should be used for locking short critical sections where the lock 
/// is contended but the data it protects are not.
/// </summary>
struct spin_wait_speculative_strategy final
    : spin_wait_strategy_traits<uint8_t, false>
{
    void reset(volatile locking_value& locking_value) const noexcept;
    signalling_bool try_lock(volatile locking_value& locking_value) const noexcept;
    void lock(volatile locking_value& locking_value) const noexcept;
    void unlock(volatile locking_value& locking_value) const noexcept;
};

//------------------------------------------------------------------------------
/**
*/
inline void 
spin_wait_speculative_strategy::reset(volatile locking_value& locking_value) const noexcept
{
    locking_value = 0;
}

} // namespace xr::threading
//------------------------------------------------------------------------------