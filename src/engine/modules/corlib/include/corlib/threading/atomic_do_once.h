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
#include "corlib/threading/atomic_backoff_helpers.h"
#include "corlib/etl/function_pointer.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

enum class do_once_state : uint8_t
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
using atomic_do_once_state = volatile do_once_state;

namespace internal
{

//! Initialization from function that do not returns anything, e.g. initializes on demand
void run_initializer(etl::action_func f, atomic_do_once_state& state);
//! Initialization from function that returns result and may need re-initialization on false result
void run_initializer(etl::prediction_func f, atomic_do_once_state& state);

}

//------------------------------------------------------------------------------
/**
*/
template< typename F >
void atomic_do_once(const F& func, atomic_do_once_state& state)
{
    using do_once_backoff = default_atomic_backoff;

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
                internal::run_initializer(func, state);
                break;
            }
        }

        backoff_do_while_function<do_once_backoff>([&state]()
        {
            return threading::atomic_fetch_acq(state) == do_once_state::pending;
        });
    }
};

} // namespace xr::threading
//------------------------------------------------------------------------------