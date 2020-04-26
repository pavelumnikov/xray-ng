// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/threading/event.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading, details)

class critical_section_raii
{
public:
    critical_section_raii(LPCRITICAL_SECTION cs) : m_cs { cs }
    {
        ::EnterCriticalSection(m_cs);
    }

    ~critical_section_raii()
    {
        ::LeaveCriticalSection(m_cs);
    }

private:
    LPCRITICAL_SECTION m_cs;
};

LPCRITICAL_SECTION to_critical_section(uint8_t const* buffer)
{
    return reinterpret_cast<LPCRITICAL_SECTION>(const_cast<uint8_t*>(buffer));
}

XR_NAMESPACE_END(xr, threading, details)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

constexpr int32_t EVENT_NOT_SIGNALED = 0;
constexpr int32_t EVENT_SIGNALED = 1;
constexpr DWORD CRIT_SPIN_COUNT_FOR_EVENT = 16;

//-----------------------------------------------------------------------------------------------------------
/**
*/
event::event(bool const initial_state) XR_NOEXCEPT
    : m_critical_section {}
    , m_condition_variable {}
    , m_num_of_waiting_threads { 0 }
    , m_value { 0 }
{
    static_assert(sizeof(m_critical_section) == sizeof(RTL_CRITICAL_SECTION),
        "m_critical_section: change type of storage");
    static_assert(sizeof(m_condition_variable) == sizeof(RTL_CONDITION_VARIABLE),
        "m_condition_variable: please change type of storage");

    auto cs = reinterpret_cast<LPCRITICAL_SECTION>(m_critical_section);
    auto cv = reinterpret_cast<PCONDITION_VARIABLE>(&m_condition_variable);

    (void)InitializeCriticalSectionAndSpinCount(cs, CRIT_SPIN_COUNT_FOR_EVENT);
    InitializeConditionVariable(cv);
    m_value = initial_state ? EVENT_SIGNALED : EVENT_NOT_SIGNALED;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
event::~event()
{
    auto cs = details::to_critical_section(m_critical_section);
    ::DeleteCriticalSection(cs);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void
event::set(bool const value) XR_NOEXCEPT
{
    if(value)
        set_event();
    else
        reset_event();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
event_wait_result
event::wait_timeout(sys::tick timeout) XR_NOEXCEPT
{
    auto cs = details::to_critical_section(m_critical_section);
    auto cv = reinterpret_cast<PCONDITION_VARIABLE>(&m_condition_variable);

    // early exit if event already signaled
    if(m_value != EVENT_NOT_SIGNALED)
        return event_wait_result::signaled;

    m_num_of_waiting_threads++;

    for(;;)
    {
        BOOL ret = ::SleepConditionVariableCS(cv, cs, static_cast<DWORD>(timeout));
        if(ret == 0)
        {
            DWORD err = ::GetLastError();
            if(err == ERROR_TIMEOUT)
                return event_wait_result::timed_out;
            else
                return event_wait_result::failed;
        }

        // https://msdn.microsoft.com/en-us/library/windows/desktop/ms686301(v=vs.85).aspx
        // Condition variables are subject to spurious wakeups (those not associated with an explicit wake)
        // and stolen wakeups (another thread manages to run before the woken thread).
        // Therefore, you should recheck a predicate (typically in a while loop) after a sleep operation returns.
        if(m_value == EVENT_SIGNALED || ret == 0)
            break;
    }

    m_num_of_waiting_threads--;
    bool is_signaled = (m_value != EVENT_NOT_SIGNALED);
    if(is_signaled)
        m_value = EVENT_NOT_SIGNALED;

    if(!is_signaled)
        return event_wait_result::still_non_signaled;

    return event_wait_result::signaled;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
signalling_bool 
event::peek() const XR_NOEXCEPT
{
    auto cs = details::to_critical_section(m_critical_section);
    details::critical_section_raii lock { cs };
    return m_value == EVENT_SIGNALED;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void event::set_event()
{
    auto cs = details::to_critical_section(m_critical_section);
    auto cv = reinterpret_cast<PCONDITION_VARIABLE>(&m_condition_variable);

    details::critical_section_raii lock { cs };
    m_value = EVENT_SIGNALED;
    if(m_num_of_waiting_threads > 0)
    {
        ::WakeConditionVariable(cv);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void event::reset_event()
{
    auto cs = details::to_critical_section(m_critical_section);
    details::critical_section_raii lock { cs };
    m_value = EVENT_NOT_SIGNALED;
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------