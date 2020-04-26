// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
template < typename T, bool IsVolatile >
struct atomic_type_helper final
{
    typedef T type;
}; // struct atomic_type_helper

//-----------------------------------------------------------------------------------------------------------
template < typename T >
struct atomic_type_helper<T,true>final
{
    typedef volatile T type;
}; // struct atomic_type_helper

//-----------------------------------------------------------------------------------------------------------
template < typename T >
struct pointer_type final
{
    typedef typename atomic_type_helper< T, eastl::is_volatile<T>::value >::type type;
}; // struct pointer_type

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
