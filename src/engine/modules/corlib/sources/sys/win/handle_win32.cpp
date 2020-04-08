// This file is a part of xray-ng engine
//

#include "corlib/sys/win/handle.h"
#include "../../os_include_win32.h"

namespace xr::sys::win
{

bool handle_traits::close(_In_ type h) noexcept
{
    return ::CloseHandle(h) != FALSE;
}

HANDLE handle_traits::get_invalid_value() noexcept
{
    return INVALID_HANDLE_VALUE;
}

} // namespace xr::sys::win
