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

#include "EASTL/numeric_limits.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

/// the time datatype
using time = double;
/// the tick datatype (one tick == 1 millisecond)
using tick = uint64_t;

constexpr tick infinite = (eastl::numeric_limits<tick>::max)();
constexpr tick min = (eastl::numeric_limits<tick>::min)();

//------------------------------------------------------------------------------
/**
 *  Current milliseconds count.
 */
tick now_milliseconds();

//------------------------------------------------------------------------------
/**
 *  Current milliseconds count.
 */
tick now_microseconds();

//------------------------------------------------------------------------------
/**
 *  Convert ticks to seconds.
 */
inline time
ticks_to_seconds(tick ticks)
{
    return static_cast<time>(ticks * 0.001);
}

//------------------------------------------------------------------------------
/**
 *  Convert seconds to ticks
 */
inline tick
seconds_to_ticks(time t)
{
    // perform correct rounding
    return static_cast<tick>((t * 1000.0) + 0.5);
}

} // namespace xr::sys
//------------------------------------------------------------------------------
