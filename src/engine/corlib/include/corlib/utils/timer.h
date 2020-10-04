// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/sys/chrono.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
class timer
{
public:
    timer() = default;

    void start();
    sys::tick get_elapsed_ms() const XR_NOEXCEPT;
    sys::time get_elapsed_sec() const XR_NOEXCEPT;

private:
    uint64_t m_start_time {};
}; // class timer

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void
timer::start()
{
    m_start_time = sys::now_microseconds();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sys::tick
timer::get_elapsed_ms() const XR_NOEXCEPT
{
    sys::tick now = sys::now_microseconds();
    return now - m_start_time;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sys::time
timer::get_elapsed_sec() const XR_NOEXCEPT
{
    return get_elapsed_ms() * 0.001; // ms to sec
}

//-----------------------------------------------------------------------------------------------------------
class inplace_timer
{
public:
    inplace_timer() XR_NOEXCEPT;
    XR_DECLARE_DEFAULT_DESTRUCTOR(inplace_timer);
    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(inplace_timer);
    XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT(inplace_timer);

    sys::tick get_elapsed_ms() const XR_NOEXCEPT;
    sys::time get_elapsed_sec() const XR_NOEXCEPT;

private:
    timer m_timer {};
}; // class timer

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline
inplace_timer::inplace_timer() XR_NOEXCEPT
    : m_timer {}
{
    m_timer.start();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sys::tick
inplace_timer::get_elapsed_ms() const XR_NOEXCEPT
{
    return m_timer.get_elapsed_ms();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sys::time
inplace_timer::get_elapsed_sec() const XR_NOEXCEPT
{
    return m_timer.get_elapsed_sec();
}

} // namespace xr::timing
//-----------------------------------------------------------------------------------------------------------
