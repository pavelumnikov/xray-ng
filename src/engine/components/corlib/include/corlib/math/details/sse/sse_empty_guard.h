// This file is a part of xray-ng engine
//

#pragma once

#include <xmmintrin.h>

namespace xr::math::details
{

/// SSE2 requires to call _mm_empty() if we are intermixing MMX integer commands with 
/// floating point arithmetics. This class guards critical code fragments where SSE2 integer
/// is used.
class empty_guard
{
public:
    empty_guard()
    {
#if !defined(XRAY_PLATFORM_64BIT)
        _mm_empty();
#endif
    }

    ~empty_guard()
    {
#if !defined(XRAY_PLATFORM_64BIT)
        _mm_empty();
#endif
    }
}; // class empty_guard

} // namespace xr::math::details
