// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/utility.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::details
{

//-----------------------------------------------------------------------------------------------------------
template<class Comparer>
class compare_key
{
    using comparer_type = Comparer;
    comparer_type const m_comparer;

public:
    constexpr compare_key(comparer_type const& comparer)
        : m_comparer(comparer)
    {}

    template <class Key, class Value>
    constexpr int operator()(eastl::pair<Key, Value> const& self,
        eastl::pair<Key, Value> const& other) const
    {
        return m_comparer(eastl::get<0>(self), eastl::get<0>(other));
    }

    template <class Key, class Value>
    constexpr int operator()(Key const& self_key,
        eastl::pair<Key, Value> const& other) const
    {
        return m_comparer(self_key, ::eastl::get<0>(other));
    }

    template <class Key, class Value>
    constexpr int operator()(::eastl::pair<Key, Value> const& self,
        Key const& other_key) const
    {
        return m_comparer(eastl::get<0>(self), other_key);
    }

    template <class Key>
    constexpr int operator()(Key const& self_key, Key const& other_key) const
    {
        return m_comparer(self_key, other_key);
    }
}; // class compare_key


} // namespace xr::utils::details
//-----------------------------------------------------------------------------------------------------------