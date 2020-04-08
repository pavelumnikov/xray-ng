// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/deque.h"

//------------------------------------------------------------------------------
namespace xr::etl::containers::std
{

//------------------------------------------------------------------------------
template<typename Type>
using deque = eastl::deque<memory::proxy::eastl_proxy_allocator, Type>;

} // namespace xr::etl::containers::std
//------------------------------------------------------------------------------