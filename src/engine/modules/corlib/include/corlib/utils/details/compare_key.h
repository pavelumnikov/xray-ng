// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/utility.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
template<class Comparer>
class compare_key
{
    using comparer_type = Comparer;
    comparer_type const m_comparer;

public:
    XR_CONSTEXPR_CPP14_OR_INLINE compare_key(comparer_type const& comparer)
        : m_comparer(comparer)
    {}

    template <class Key, class Value>
    XR_CONSTEXPR_CPP14_OR_INLINE int operator()(eastl::pair<Key, Value> const& self,
        eastl::pair<Key, Value> const& other) const
    {
        return m_comparer(eastl::get<0>(self), eastl::get<0>(other));
    }

    template <class Key, class Value>
    XR_CONSTEXPR_CPP14_OR_INLINE int operator()(Key const& self_key,
        eastl::pair<Key, Value> const& other) const
    {
        return m_comparer(self_key, ::eastl::get<0>(other));
    }

    template <class Key, class Value>
    XR_CONSTEXPR_CPP14_OR_INLINE int operator()(::eastl::pair<Key, Value> const& self,
        Key const& other_key) const
    {
        return m_comparer(eastl::get<0>(self), other_key);
    }

    template <class Key>
    XR_CONSTEXPR_CPP14_OR_INLINE int operator()(Key const& self_key, Key const& other_key) const
    {
        return m_comparer(self_key, other_key);
    }
}; // class compare_key


XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------