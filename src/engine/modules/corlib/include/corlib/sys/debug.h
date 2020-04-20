// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

void assert_format(const char* exp, const char* fmt, const char* file, int line, va_list args);

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------