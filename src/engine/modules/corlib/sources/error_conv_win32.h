// This file is a part of xray-ng engine
//

#pragma once

#include "os_include_win32.h"

//------------------------------------------------------------------------------
namespace xr
{

//------------------------------------------------------------------------------
/**
*/
bool convert_last_error_to_char(char* buffer, size_t buffer_size);

//------------------------------------------------------------------------------
/**
*/
bool convert_last_error_to_char(wchar_t* buffer, size_t buffer_size);

//------------------------------------------------------------------------------
/**
*/
bool convert_to_char(DWORD dwCode, char* buffer, size_t buffer_size);

//------------------------------------------------------------------------------
/**
*/
bool convert_to_wchar(DWORD dwCode, wchar_t* buffer, size_t buffer_size);

} // namespace xr
//------------------------------------------------------------------------------