// This file is a part of xray-ng engine
//

#include "corlib/sys/chrono.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys)

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint64_t get_frequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
tick now_milliseconds()
{
    LARGE_INTEGER qpcnt;
    int rval = QueryPerformanceCounter(&qpcnt);
    XR_DEBUG_ASSERTION_MSG(rval, "QueryPerformanceCounter failed");
    return (qpcnt.QuadPart * uint64_t(1000)) / get_frequency();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
tick now_microseconds()
{
    LARGE_INTEGER qpcnt;
    int rval = QueryPerformanceCounter(&qpcnt);
    XR_DEBUG_ASSERTION_MSG(rval, "QueryPerformanceCounter failed");
    return (qpcnt.QuadPart * uint64_t(1000000)) / get_frequency();
}

XR_NAMESPACE_END(xr, sys)
//-----------------------------------------------------------------------------------------------------------
