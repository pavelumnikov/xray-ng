// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/string_map.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename T>
using string_map = ::eastl::string_map<T, eastl::str_less<const char*>, memory::proxy::eastl_proxy_allocator>;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------