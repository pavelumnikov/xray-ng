// This file is a part of xray-ng engine
//

#include "error_conv_win32.h"
#include <cassert>

//------------------------------------------------------------------------------
namespace xr
{

//------------------------------------------------------------------------------
/**
*/
bool convert_last_error_to_char(char* buffer, size_t buffer_size)
{
    DWORD dw = ::GetLastError();
    return convert_to_char(dw, buffer, buffer_size);
}

//------------------------------------------------------------------------------
/**
*/
bool convert_last_error_to_char(wchar_t* buffer, size_t buffer_size)
{
    DWORD dw = ::GetLastError();
    return convert_to_wchar(dw, buffer, buffer_size);
}

//------------------------------------------------------------------------------
/**
*/
bool convert_to_char(DWORD dw, char* buffer, size_t buffer_size)
{
    assert(buffer);
    assert(buffer_size > 0);

    ::FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, 
        static_cast<DWORD>(buffer_size), nullptr);

    return true;
}

//------------------------------------------------------------------------------
/**
*/
bool convert_to_wchar(DWORD dw, wchar_t* buffer, size_t buffer_size)
{
    assert(buffer);
    assert(buffer_size > 0);

    ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer,
        static_cast<DWORD>(buffer_size), nullptr);

    return true;
}

} // namespace xr
//------------------------------------------------------------------------------