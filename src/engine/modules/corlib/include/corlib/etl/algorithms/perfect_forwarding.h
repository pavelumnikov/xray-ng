// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

// workaround: std utility functions aren't constexpr yet
template <class T>
constexpr T&&
constexpr_forward(eastl::remove_reference_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&&
constexpr_forward(eastl::remove_reference_t<T>&& t) noexcept
{
    static_assert(!eastl::is_lvalue_reference<T>::value, "!!");
    return static_cast<T&&>(t);
}

template <class T>
constexpr eastl::remove_reference_t<T>&&
constexpr_move(T&& t) noexcept
{
    return static_cast<eastl::remove_reference_t<T>&&>(t);
}

} // namespace xr
//-----------------------------------------------------------------------------------------------------------
