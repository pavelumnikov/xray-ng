// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/unordered_set.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename Key, typename Value>
using unordered_set = ::eastl::unordered_set<memory::proxy::eastl_proxy_allocator, T>;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------