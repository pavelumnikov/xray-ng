// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys)

void assert_format(const char* exp, const char* fmt, const char* file, int line, va_list args);

XR_NAMESPACE_END(xr, sys)
//-----------------------------------------------------------------------------------------------------------