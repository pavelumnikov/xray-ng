// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/list.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename T>
using list = eastl::list<T, memory::proxy::eastl_proxy_allocator>;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------