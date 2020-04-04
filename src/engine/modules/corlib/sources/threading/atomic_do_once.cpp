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

#include "corlib/threading/atomic_do_once.h"

//------------------------------------------------------------------------------
namespace xr::threading::internal
{

//------------------------------------------------------------------------------
/**
*/
void run_initializer(etl::action_func f, atomic_do_once_state& state)
{
    // Run function handle
    f();
    // Do atomic exchanging of value
    threading::atomic_store_rel(state, do_once_state::executed);
}

//------------------------------------------------------------------------------
/**
*/
void run_initializer(etl::prediction_func f, atomic_do_once_state& state)
{
    // Run function handle
    auto const result = f();
    // Do atomic exchanging of value
    result ? threading::atomic_store_rel(state, do_once_state::executed) :
        threading::atomic_store_rel(state, do_once_state::uninitialized);
}

} // namespace xr::threading::internal
//------------------------------------------------------------------------------