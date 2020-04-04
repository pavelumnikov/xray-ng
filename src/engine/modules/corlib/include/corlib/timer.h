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