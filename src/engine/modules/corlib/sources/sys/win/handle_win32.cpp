// This file is a part of xray-ng engine
//

#include "corlib/sys/win/handle.h"
#include "../../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys, win)

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool handle_traits::close(_In_ type h) XR_NOEXCEPT
{
    return ::CloseHandle(h) != FALSE;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
HANDLE handle_traits::get_invalid_value() XR_NOEXCEPT
{
    return INVALID_HANDLE_VALUE;
}

XR_NAMESPACE_END(xr, sys, win)
//-----------------------------------------------------------------------------------------------------------
