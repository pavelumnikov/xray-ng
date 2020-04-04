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

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/threading/spin_wait_speculative_locking_strategy.h"
#include "corlib/threading/atomic_backoff_helpers.h"
#include "../os_include_win32.h"

extern "C" int8_t _TSXTryLockTransactive( volatile void* ptr );
extern "C" void _TSXUnlockTransactive( volatile void* ptr );

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
/**
*/
void 
spin_wait_speculative_strategy::lock(volatile locking_value& locking_value) const noexcept
{
    for(;;)
    {
        backoff_while_equals< default_atomic_backoff >(locking_value, 1);

        if(::_TSXTryLockTransactive(&locking_value))
            return;

        // Another thread acquired the lock "for real".
        // To avoid the "lemming" effect, we abort the transaction.
        ::_mm_pause();
    }
}

//------------------------------------------------------------------------------
/**
*/
signalling_bool
spin_wait_speculative_strategy::try_lock(volatile locking_value& locking_value) const noexcept
{
    auto const result = ::_TSXTryLockTransactive(&locking_value) != 0;

    // to avoid the "lemming" effect, we need to abort the transaction
    // if  _TSXTryLockTransactive returns false (i.e., someone else
    // has acquired the mutex non-speculatively).
    if(!result)
        _mm_pause();

    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
spin_wait_speculative_strategy::unlock(volatile locking_value& locking_value) const noexcept
{
    ::_TSXUnlockTransactive(&locking_value);
}

} // namespace xr::threading
//------------------------------------------------------------------------------