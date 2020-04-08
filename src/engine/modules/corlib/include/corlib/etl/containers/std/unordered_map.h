// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/unordered_map.h"

//------------------------------------------------------------------------------
namespace xr::etl::containers::std
{

//------------------------------------------------------------------------------
using unodered_map_allocator = memory::proxy::eastl_proxy_allocator;

//------------------------------------------------------------------------------
template<
    typename Key, 
    typename Value, 
    typename Hash = eastl::hash<Key>,
    typename Predicate = eastl::equal_to<Key>
>
using unordered_map = eastl::unordered_map<unodered_map_allocator, 
    Key, Value, Hash, Predicate>;

//------------------------------------------------------------------------------
template<
    typename Key,
    typename Value,
    typename Hash = eastl::hash<Key>,
    typename Predicate = eastl::equal_to<Key>
>
using unordered_multimap = eastl::unordered_multimap<unodered_map_allocator,
    Key, Value, Hash, Predicate>;

} // namespace xr::etl::containers::std
//------------------------------------------------------------------------------