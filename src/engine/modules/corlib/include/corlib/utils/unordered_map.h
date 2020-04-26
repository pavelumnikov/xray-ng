// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/unordered_map.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename Key, typename Value>
using unordered_map = ::eastl::unordered_map<memory::proxy::eastl_proxy_allocator, Key, Value>;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------