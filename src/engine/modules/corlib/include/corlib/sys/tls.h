// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/string_view.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
using tls_handle = uint32_t;

uintptr_t constexpr invalid_thread_local_storage =
    (eastl::numeric_limits<tls_handle>::max)();

//------------------------------------------------------------------------------
/**
*/
tls_handle create_thread_local();

//------------------------------------------------------------------------------
/**
*/
bool set_tls_data(tls_handle const tls, void* data);

//------------------------------------------------------------------------------
/**
*/
void* get_tls_data(tls_handle const tls);

//------------------------------------------------------------------------------
/**
*/
template<typename T>
T* get_tls_typed_data(tls_handle const tls)
{
    return reinterpret_cast<T*>(sys::get_tls_data(tls));
}

//------------------------------------------------------------------------------
/**
*/
void destroy_thread_local(tls_handle const tls);

} // namespace xr::sys
//------------------------------------------------------------------------------