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

#include "corlib/signalling_bool.h"
#include "corlib/sys/chrono.h"
#include "corlib/sys/win/min_windows.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
enum class event_wait_result : uint8_t
{
    failed,
    signaled,
    still_non_signaled,
    timed_out
}; // enum class event_wait_result


//------------------------------------------------------------------------------
class event
{
public:
    explicit event(bool initial_state = false) noexcept;
    ~event();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(event);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(event);

    // Signal the event.
    void set(bool value) const noexcept;

    // Wait for the event to become signaled.
    event_wait_result wait() const noexcept;

    // Waits for the event to become signaled with a specified timeout
    // in milliseconds.
    // If the method times out it will return false,
    // if the event becomes signaled within the timeout it will return true.
    event_wait_result wait_timeout(sys::tick timeout) const noexcept;

    // This checks if the event is signaled and returns immediately.
    signalling_bool peek() const noexcept;

    void* get_handle() const noexcept;

private:
    using handle = sys::win::HANDLE;
    handle m_handle; // event base handle
};

//------------------------------------------------------------------------------
/**
*/
inline event_wait_result 
event::wait() const noexcept
{
    return this->wait_timeout(sys::infinite);
}

//------------------------------------------------------------------------------
/**
*/
inline void*
event::get_handle() const noexcept
{
    return this->m_handle;
}

} // namespace xr::threading
//------------------------------------------------------------------------------