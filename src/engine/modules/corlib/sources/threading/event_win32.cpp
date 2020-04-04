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

#include "corlib/threading/event.h"
#include "../os_include_win32.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
/**
*/
event::event(bool const initial_state) noexcept
    : m_handle(nullptr)
{
    static_assert(sizeof(m_handle) == sizeof(HANDLE), "please change type of storage");
    auto const is_initial_state = static_cast<BOOL>(initial_state);
    auto const handle = CreateEventA(nullptr, 0, is_initial_state, nullptr);
    m_handle = handle;
}

//------------------------------------------------------------------------------
/**
*/
event::~event()
{
    XR_DEBUG_ASSERTION(m_handle);

    CloseHandle(m_handle);
    m_handle = nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void
event::set(bool const value) const noexcept
{
    XR_DEBUG_ASSERTION(this->m_handle);

    if(value)
        SetEvent(m_handle);
    else
        ResetEvent(m_handle);
}

//------------------------------------------------------------------------------
/**
*/
event_wait_result
event::wait_timeout(sys::tick timeout) const noexcept
{
    XR_DEBUG_ASSERTION(this->m_handle);

    auto constexpr alertable = TRUE; // all threads must be alertable

    auto const result = WaitForSingleObjectEx(
        m_handle, static_cast<uint32_t>(timeout), alertable);

    switch(result)
    {
        case WAIT_OBJECT_0:
            return event_wait_result::signaled;

        case WAIT_TIMEOUT:
            return event_wait_result::timed_out;

        case WAIT_FAILED:
            return event_wait_result::failed;

        default:
            return event_wait_result::still_non_signaled;
    }
}

//------------------------------------------------------------------------------
/**
*/
signalling_bool 
event::peek() const noexcept
{
    XR_DEBUG_ASSERTION(m_handle);

    DWORD constexpr milliseconds = 0;
    auto constexpr alertable = TRUE; // all threads must be alertable

    auto const result = WaitForSingleObjectEx(
        m_handle, milliseconds, alertable);

    return WAIT_TIMEOUT != result;
}

} // namespace xr::threading
//------------------------------------------------------------------------------