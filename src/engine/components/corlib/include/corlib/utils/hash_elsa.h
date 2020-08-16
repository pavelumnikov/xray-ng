// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
template<typename T> 
struct hash_elsa final
{
    XR_STATIC_ASSERT(eastl::is_integral_v<T> || eastl::is_enum<T>::value,
        "only supports integral types, specialize for other types");

    XR_CONSTEXPR_CPP14_OR_INLINE size_t operator()(T const& value, size_t seed) const;
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE size_t 
hash_elsa<T>::operator()(T const& value, size_t const seed) const
{
    size_t key = seed ^ static_cast<size_t>(value);
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
