// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/threading/event.h"
#include "corlib/threading/interlocked.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
class lightweight_event
{
public:
    explicit lightweight_event(bool initial_state = false) XR_NOEXCEPT;
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(lightweight_event);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(lightweight_event);

    // Signal the event.
    void set(bool value) XR_NOEXCEPT;

    // Wait for the event to become signaled.
    event_wait_result wait() XR_NOEXCEPT;

    // Waits for the event to become signaled with a specified timeout
    // in milliseconds.
    // If the method times out it will return false,
    // if the event becomes signaled within the timeout it will return true.
    event_wait_result wait_timeout(sys::tick timeout) XR_NOEXCEPT;

    // This checks if the event is signaled and returns immediately.
    signalling_bool peek() const XR_NOEXCEPT;

private:
    volatile bool m_signaled;
    sys::win::BOOLEAN m_handle; // event base handle
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline event_wait_result
lightweight_event::wait() XR_NOEXCEPT
{
    return this->wait_timeout(sys::infinite);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
