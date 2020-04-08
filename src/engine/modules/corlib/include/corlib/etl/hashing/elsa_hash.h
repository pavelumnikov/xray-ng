// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::etl::hashing
{

//------------------------------------------------------------------------------
template<typename T> 
struct elsa final
{
    static_assert(eastl::is_integral_v<T> || eastl::is_enum<T>::value,
        "only supports integral types, specialize for other types");

    constexpr size_t operator()(T const &value, size_t seed) const;
};

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr size_t 
elsa<T>::operator()(T const &value, size_t const seed) const
{
    auto key = seed ^ static_cast<size_t>(value);

    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);

    return key;
}

} // namespace xr::etl::hashing
//------------------------------------------------------------------------------