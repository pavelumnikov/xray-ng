/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/sys/dll.h"
#include "../os_include_win32.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
/**
*/
pvoid dll_load(eastl::string_view str)
{
    if(str.empty())
        return nullptr;

    auto const handle = ::LoadLibraryA(str.data());
    return static_cast<pvoid>(handle);
}

//------------------------------------------------------------------------------
/**
*/
void dll_unload(pvoid library)
{
    if(!library)
        return;

    auto const handle = static_cast<HMODULE>(library);
    ::FreeLibrary(handle);
}

//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------