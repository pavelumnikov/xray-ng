// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"
#include "corlib/sys/chrono.h"

//------------------------------------------------------------------------------
namespace xr
{

//------------------------------------------------------------------------------
class timer
{
public:
    XR_DECLARE_DEFAULT_CONSTEXPR_CLASS(timer);

    void start();
    sys::tick get_elapsed_ms() const noexcept;
    sys::time get_elapsed_sec() const noexcept;

private:
    uint64_t m_start_time {};
}; // class timer

//------------------------------------------------------------------------------
/**
 */
inline void
timer::start()
{
    m_start_time = sys::now_microseconds();
}

//------------------------------------------------------------------------------
/**
 */
inline sys::tick
timer::get_elapsed_ms() const noexcept
{
    sys::tick now = sys::now_microseconds();
    return now - m_start_time;
}

//------------------------------------------------------------------------------
/**
 */
inline sys::time
timer::get_elapsed_sec() const noexcept
{
    return get_elapsed_ms() * 0.001; // ms to sec
}

//------------------------------------------------------------------------------
class inplace_timer
{
public:
    inplace_timer() noexcept;
    XR_DECLARE_DEFAULT_DESTRUCTOR(inplace_timer);
    XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT(inplace_timer);
    XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT(inplace_timer);

    sys::tick get_elapsed_ms() const noexcept;
    sys::time get_elapsed_sec() const noexcept;

private:
    timer m_timer {};
}; // class timer

//------------------------------------------------------------------------------
/**
 */
inline
inplace_timer::inplace_timer() noexcept
    : m_timer {}
{
    m_timer.start();
}

//------------------------------------------------------------------------------
/**
 */
inline sys::tick
inplace_timer::get_elapsed_ms() const noexcept
{
    return m_timer.get_elapsed_ms();
}

//------------------------------------------------------------------------------
/**
 */
inline sys::time
inplace_timer::get_elapsed_sec() const noexcept
{
    return m_timer.get_elapsed_sec();
}

} // namespace xr::timing
//------------------------------------------------------------------------------