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

#include "corlib/threading/lightweight_event.h"
#include "corlib/threading/interlocked.h"
#include "../os_include_win32.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
/**
*/
lightweight_event::lightweight_event(bool const initial_state) noexcept
    : m_signaled { initial_state }
    , m_handle {}
{
}

//------------------------------------------------------------------------------
/**
*/
void
lightweight_event::set(bool const value) noexcept
{
    XR_DEBUG_ASSERTION(m_handle);

    if(value)
        WakeByAddressAll(&m_handle);
    else
        threading::atomic_store_rel(m_signaled, false);
}

//------------------------------------------------------------------------------
/**
*/
event_wait_result
lightweight_event::wait_timeout(sys::tick timeout) noexcept
{
    XR_DEBUG_ASSERTION(this->m_handle);

    BOOLEAN signaled = false;

    while(!m_signaled)
    {
        if(!WaitOnAddress(&m_handle, &signaled, sizeof(BOOLEAN), static_cast<uint32_t>(timeout)))
            return event_wait_result::timed_out;
    }

    return event_wait_result::signaled;
}

//------------------------------------------------------------------------------
/**
*/
signalling_bool
lightweight_event::peek() const noexcept
{
    XR_DEBUG_ASSERTION(m_handle);
    return threading::atomic_fetch_acq(m_signaled);
}

} // namespace xr::threading
//------------------------------------------------------------------------------