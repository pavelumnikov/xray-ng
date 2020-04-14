// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/numeric_limits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

/// the time datatype
using time = double;
/// the tick datatype (one tick == 1 millisecond)
using tick = uint64_t;

constexpr tick infinite = (eastl::numeric_limits<tick>::max)();
constexpr tick min = (eastl::numeric_limits<tick>::min)();

//-----------------------------------------------------------------------------------------------------------
/**
 *  Current milliseconds count.
 */
tick now_milliseconds();

//-----------------------------------------------------------------------------------------------------------
/**
 *  Current milliseconds count.
 */
tick now_microseconds();

//-----------------------------------------------------------------------------------------------------------
/**
 *  Convert ticks to seconds.
 */
inline time
ticks_to_seconds(tick ticks)
{
    return static_cast<time>(ticks * 0.001);
}

//-----------------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------------
