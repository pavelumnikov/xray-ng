// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/sys/dll.h"
#include "../os_include_win32.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid dll_load(eastl::string_view str)
{
    if(str.empty())
        return nullptr;

    auto const handle = ::LoadLibraryA(str.data());
    return static_cast<pvoid>(handle);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void dll_unload(pvoid library)
{
    if(!library)
        return;

    auto const handle = static_cast<HMODULE>(library);
    ::FreeLibrary(handle);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid dll_require_symbol(pvoid library, eastl::string_view str)
{
    if(!library)
        return nullptr;

    if(str.empty())
        return nullptr;

    auto const handle = static_cast<HMODULE>(library);
    return reinterpret_cast<pvoid>(::GetProcAddress(handle, str.data()));
}

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------
