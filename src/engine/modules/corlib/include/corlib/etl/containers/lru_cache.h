/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

#include "corlib/signalling_bool.h"
#include "corlib/etl/containers/std/unordered_map.h"
#include "corlib/memory/memory_allocator_base.h"
#include "EASTL/list.h"

//------------------------------------------------------------------------------
namespace xr::etl::containers
{

//------------------------------------------------------------------------------
template<typename Key, typename Value>
class lru_cache
{
public:
    using key_value_pair_t = typename ::eastl::pair<Key, Value>;
    using key_type = typename key_value_pair_t::first_type;
    using value_type = typename key_value_pair_t::second_type;
    using size_type = size_t;

public:
    lru_cache(size_type max_size, memory::base_allocator& alloc);

    void put(const key_type& key, const value_type& value);
    signalling_bool try_get(const key_type& key, value_type& output);

    signalling_bool exists(const key_type& key) const;
    constexpr size_type size() const;

private:
    using list_iterator = typename eastl::list<key_value_pair_t>::iterator;
    using cache_items_list_t = eastl::list<key_value_pair_t>;
    using cache_items_map_t = etl::containers::std::unordered_map<Key, list_iterator>;

    memory::proxy::eastl_proxy_allocator m_eastl_proxy_allocator;
    cache_items_list_t m_cache_items_list;
    cache_items_map_t m_cache_items_map;
    size_type m_max_size;
};

//------------------------------------------------------------------------------
/**
*/
template<typename Key, typename Value>
lru_cache<Key, Value>::lru_cache(size_type max_size, memory::base_allocator& alloc)
    : m_eastl_proxy_allocator(alloc)
    , m_cache_items_list(m_eastl_proxy_allocator)
    , m_cache_items_map(m_eastl_proxy_allocator)
    , m_max_size(max_size)
{}

//------------------------------------------------------------------------------
/**
*/
template<typename Key, typename Value>
inline void
lru_cache<Key, Value>::put(const key_type& key, const value_type& value)
{
    auto it = m_cache_items_map.find(key);
    m_cache_items_list.push_front(key_value_pair_t(key, value));

    if(it != m_cache_items_map.end())
    {
        m_cache_items_list.erase(it->second);
        m_cache_items_map.erase(it);
    }
    m_cache_items_map[key] = m_cache_items_list.begin();

    if(m_cache_items_map.size() > m_max_size)
    {
        auto last = m_cache_items_list.end();
        last--;
        m_cache_items_map.erase(last->first);
        m_cache_items_list.pop_back();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<typename Key, typename Value>
inline signalling_bool 
lru_cache<Key, Value>::try_get(const key_type& key, value_type& output)
{
    auto it = m_cache_items_map.find(key);
    if(it == m_cache_items_map.end())
        return false;

    m_cache_items_list.splice(
        m_cache_items_list.begin(), m_cache_items_list, it->second);

    output = it->second->second;
    return true;
}

//------------------------------------------------------------------------------
/**
*/
template<typename Key, typename Value>
inline signalling_bool 
lru_cache<Key, Value>::exists(const key_type& key) const
{
    return m_cache_items_map.find(key) != ::eastl::end(m_cache_items_map);
}

//------------------------------------------------------------------------------
/**
*/
template<typename Key, typename Value>
constexpr lru_cache<Key, Value>::size_type
lru_cache<Key, Value>::size() const
{
    return ::eastl::size(m_cache_items_map);
}

} // namespace xr::etl::containers
//------------------------------------------------------------------------------