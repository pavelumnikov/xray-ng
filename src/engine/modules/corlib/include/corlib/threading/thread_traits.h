// This file is a part of xray-ng engine
//

#pragma once

#include "EASTL/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

template < typename T, bool IsVolatile >
struct atomic_type_helper final
{
    using type = T;
}; // struct atomic_type_helper

template < typename T >
struct atomic_type_helper<T,true>final
{
    using type = eastl::add_volatile_t<T>;
}; // struct atomic_type_helper

template < typename T >
struct pointer_type final
{
    using type = typename atomic_type_helper< T, eastl::is_volatile<T>::value >::type;
}; // struct pointer_type

template < typename T >
using pointer_type_t = pointer_type<T>;

} // namespace xr::threading
//------------------------------------------------------------------------------