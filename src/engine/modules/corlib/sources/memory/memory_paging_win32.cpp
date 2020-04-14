// This file is a part of xray-ng engine
//

#include "corlib/memory/memory_paging.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t system_page_size()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
}

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------
