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

#include "corlib/sys/tls.h"
#include "../os_include_win32.h"
#include <VersionHelpers.h>
#include <process.h>
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
/**
*/
tls_handle create_thread_local()
{
    auto const tls_index = TlsAlloc();

    if(tls_index == TLS_OUT_OF_INDEXES)
        return static_cast<tls_handle>(invalid_thread_local_storage);

    return static_cast<tls_handle>(tls_index);
}

//------------------------------------------------------------------------------
/**
*/
bool set_tls_data(tls_handle const tls, void* data)
{
    XR_DEBUG_ASSERTION_MSG(tls != invalid_thread_local_storage,
        "Thread-local storage index cannot be invalid");

    auto const tls_index = static_cast<DWORD>(tls);
    return TlsSetValue(tls_index, data) != 0;
}

//------------------------------------------------------------------------------
/**
*/
void* get_tls_data(tls_handle const tls)
{
    XR_DEBUG_ASSERTION_MSG(tls != invalid_thread_local_storage,
        "Thread-local storage index cannot be invalid");

    auto const tls_index = static_cast<DWORD>(tls);
    return TlsGetValue(tls_index);
}

//------------------------------------------------------------------------------
/**
*/
void destroy_thread_local(tls_handle const tls)
{
    XR_DEBUG_ASSERTION_MSG(tls != invalid_thread_local_storage,
        "Thread-local storage index cannot be invalid");

    TlsFree(static_cast<tls_handle>(tls));
}

} // namespace xr::sys
//------------------------------------------------------------------------------