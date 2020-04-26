// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename Type0, typename Type1 >
inline void 
backoff_while_equals(Type0& location, Type1 value) XR_NOEXCEPT
{
    BackoffType backoff;
    while(location == value)
        backoff.pause();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename Type0, typename Type1 >
inline void 
backoff_while_not_equals(Type0& location, Type1 value) XR_NOEXCEPT
{
    BackoffType backoff;
    while(location != value)
        backoff.pause();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename BackoffFunction >
inline void 
backoff_do_while_function(BackoffFunction&& function) XR_NOEXCEPT
{
    BackoffType backoff;
    do
    {
        backoff.pause();
    } while(function());
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename BackoffFunction >
inline void
backoff_while_function(BackoffFunction&& function) XR_NOEXCEPT
{
    BackoffType backoff;
    while(function())
        backoff.pause();
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
