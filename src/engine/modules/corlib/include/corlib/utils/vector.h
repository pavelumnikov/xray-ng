// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/vector.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename T>
using vector = ::eastl::vector<T, memory::proxy::eastl_proxy_allocator>;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------