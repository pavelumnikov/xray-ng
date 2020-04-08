// This file is a part of xray-ng engine
//

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