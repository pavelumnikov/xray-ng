// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/threading/event.h"
#include "corlib/threading/interlocked.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::threading
{

//-----------------------------------------------------------------------------------------------------------
class lightweight_event
{
public:
    explicit lightweight_event(bool initial_state = false) noexcept;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(lightweight_event);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(lightweight_event);

    // Signal the event.
    void set(bool value) noexcept;

    // Wait for the event to become signaled.
    event_wait_result wait() noexcept;

    // Waits for the event to become signaled with a specified timeout
    // in milliseconds.
    // If the method times out it will return false,
    // if the event becomes signaled within the timeout it will return true.
    event_wait_result wait_timeout(sys::tick timeout) noexcept;

    // This checks if the event is signaled and returns immediately.
    signalling_bool peek() const noexcept;

private:
    volatile bool m_signaled;
    sys::win::BOOLEAN m_handle; // event base handle
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline event_wait_result
lightweight_event::wait() noexcept
{
    return this->wait_timeout(sys::infinite);
}

} // namespace xr::threading
//-----------------------------------------------------------------------------------------------------------
