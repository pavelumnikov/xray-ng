// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/map.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::containers::std
{

//-----------------------------------------------------------------------------------------------------------
template<typename Key, typename Value, typename Compare = eastl::less<Key>>
using map = eastl::map<Key, Value, Compare, memory::proxy::eastl_proxy_allocator>;

} // namespace xr::etl::containers::std
//-----------------------------------------------------------------------------------------------------------
