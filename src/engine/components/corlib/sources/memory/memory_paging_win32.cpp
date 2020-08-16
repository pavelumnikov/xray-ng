// This file is a part of xray-ng engine
//

#include "corlib/memory/memory_paging.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t system_page_size()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
