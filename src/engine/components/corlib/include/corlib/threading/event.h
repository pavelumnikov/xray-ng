// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_types.h"
#include "corlib/sys/chrono.h"
#include "corlib/sys/win/min_windows.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
enum class event_wait_result
{
    failed,
    signaled,
    still_non_signaled,
    timed_out
};

//-----------------------------------------------------------------------------------------------------------
class event
{
public:
    explicit event(bool initial_state = false) XR_NOEXCEPT;
    ~event();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(event);

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
    typedef sys::win::RTL_CONDITION_VARIABLE handle;

    void set_event();
    void reset_event();

#if defined(XRAY_PLATFORM_64BIT)
    uint8_t m_critical_section[40];
#else
    uint8_t m_critical_section[24];
#endif // defined(XRAY_PLATFORM_64BIT)

    handle m_condition_variable;
    atomic_uint32 m_num_of_waiting_threads;
    atomic_int32 m_value;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline event_wait_result 
event::wait() XR_NOEXCEPT
{
    return wait_timeout(sys::infinite);
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
