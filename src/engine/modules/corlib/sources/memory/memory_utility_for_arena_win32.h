// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t mem_usage(HANDLE heap_handle, uint32_t* used, uint32_t* freed);

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
