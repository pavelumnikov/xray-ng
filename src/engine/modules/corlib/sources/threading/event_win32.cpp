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
namespace xr::threading::details
{

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

}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::threading
{

constexpr int32_t EVENT_NOT_SIGNALED = 0;
constexpr int32_t EVENT_SIGNALED = 1;
constexpr DWORD CRIT_SPIN_COUNT_FOR_EVENT = 16;

//------------------------------------------------------------------------------
/**
*/
event::event(bool const initial_state) noexcept
    : m_critical_section {}
    , m_condition_variable { nullptr }
    , m_num_of_waiting_threads { 0 }
    , m_value { 0 }
{
    static_assert(sizeof(m_critical_section) == sizeof(CRITICAL_SECTION),
        "m_critical_section: change type of storage");
    static_assert(sizeof(m_condition_variable) == sizeof(HANDLE),
        "m_condition_variable: please change type of storage");

    auto cs = reinterpret_cast<LPCRITICAL_SECTION>(m_critical_section);
    auto cv = reinterpret_cast<PCONDITION_VARIABLE>(m_condition_variable);

    (void)InitializeCriticalSectionAndSpinCount(cs, CRIT_SPIN_COUNT_FOR_EVENT);
    InitializeConditionVariable(cv);
    m_value = initial_state ? EVENT_SIGNALED : EVENT_NOT_SIGNALED;
}

//------------------------------------------------------------------------------
/**
*/
event::~event()
{
    auto cs = details::to_critical_section(m_critical_section);
    ::DeleteCriticalSection(cs);
}

//------------------------------------------------------------------------------
/**
*/
void
event::set(bool const value) noexcept
{
    if(value)
        set_event();
    else
        reset_event();
}

//------------------------------------------------------------------------------
/**
*/
event_wait_result
event::wait_timeout(sys::tick timeout) noexcept
{
    auto cs = details::to_critical_section(m_critical_section);
    auto cv = reinterpret_cast<PCONDITION_VARIABLE>(m_condition_variable);

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

//------------------------------------------------------------------------------
/**
*/
signalling_bool 
event::peek() const noexcept
{
    auto cs = details::to_critical_section(m_critical_section);
    details::critical_section_raii lock { cs };
    return m_value == EVENT_SIGNALED;
}

//------------------------------------------------------------------------------
/**
*/
inline void event::set_event()
{
    auto cs = details::to_critical_section(m_critical_section);
    auto cv = reinterpret_cast<PCONDITION_VARIABLE>(m_condition_variable);

    details::critical_section_raii lock { cs };
    m_value = EVENT_SIGNALED;
    if(m_num_of_waiting_threads > 0)
    {
        ::WakeConditionVariable(cv);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline void event::reset_event()
{
    auto cs = details::to_critical_section(m_critical_section);
    details::critical_section_raii lock { cs };
    m_value = EVENT_NOT_SIGNALED;
}

} // namespace xr::threading
//------------------------------------------------------------------------------