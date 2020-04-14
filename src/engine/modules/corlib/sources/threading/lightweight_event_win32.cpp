// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/threading/lightweight_event.h"
#include "corlib/threading/interlocked.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::threading
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
lightweight_event::lightweight_event(bool const initial_state) noexcept
    : m_signaled { initial_state }
    , m_handle {}
{
}

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
/**
*/
signalling_bool
lightweight_event::peek() const noexcept
{
    XR_DEBUG_ASSERTION(m_handle);
    return threading::atomic_fetch_acq(m_signaled);
}

} // namespace xr::threading
//-----------------------------------------------------------------------------------------------------------
