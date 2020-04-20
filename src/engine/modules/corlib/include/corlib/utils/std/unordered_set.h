// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/unordered_set.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::std
{

//-----------------------------------------------------------------------------------------------------------
using unordered_set_allocator = memory::proxy::eastl_proxy_allocator;

//-----------------------------------------------------------------------------------------------------------
template<
    typename Key,
    typename Hash = eastl::hash<Key>,
    typename Predicate = eastl::equal_to<Key>
>
using unordered_set = eastl::unordered_set<Key, Hash, Predicate, unordered_set_allocator>;

//-----------------------------------------------------------------------------------------------------------
template<
    typename Key,
    typename Hash = eastl::hash<Key>,
    typename Predicate = eastl::equal_to<Key>
>
using unordered_multiset = eastl::unordered_multiset<Key, Hash, Predicate, unordered_set_allocator>;

} // namespace xr::utils::std
//-----------------------------------------------------------------------------------------------------------
