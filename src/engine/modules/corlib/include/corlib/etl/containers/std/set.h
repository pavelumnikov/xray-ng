// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/set.h"

//------------------------------------------------------------------------------
namespace xr::etl::containers::std
{

//------------------------------------------------------------------------------
template<typename Key, typename Compare = eastl::less<Key>>
using set = eastl::set<Key, Compare, memory::proxy::eastl_proxy_allocator>;

//------------------------------------------------------------------------------
template<typename Key, typename Compare = eastl::less<Key>>
using multiset = eastl::multiset<Key, Compare, memory::proxy::eastl_proxy_allocator>;

} // namespace xr::etl::containers::std
//------------------------------------------------------------------------------