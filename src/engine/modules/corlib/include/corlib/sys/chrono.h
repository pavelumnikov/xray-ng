// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys)

/// the time datatype
typedef double time;
/// the tick datatype (one tick == 1 millisecond)
typedef uint64_t tick;

XR_CONSTEXPR_CPP14_OR_CONST tick infinite = UINT64_MAX;
XR_CONSTEXPR_CPP14_OR_CONST tick min = 0;

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

XR_NAMESPACE_END(xr, sys)
//-----------------------------------------------------------------------------------------------------------
