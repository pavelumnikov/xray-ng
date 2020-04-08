// This file is a part of xray-ng engine
//

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
    using handle = sys::win::HANDLE;

    void set_event();
    void reset_event();

#if defined(XRAY_PLATFORM_64BIT)
    uint8_t m_critical_section[40];
#else
    uint8_t m_critical_section[24];
#endif // defined(XRAY_PLATFORM_64BIT)

    handle m_condition_variable;

    volatile uint32_t m_num_of_waiting_threads;
    volatile int32_t m_value;
};

//------------------------------------------------------------------------------
/**
*/
inline event_wait_result 
event::wait() noexcept
{
    return wait_timeout(sys::infinite);
}

} // namespace xr::threading
//------------------------------------------------------------------------------