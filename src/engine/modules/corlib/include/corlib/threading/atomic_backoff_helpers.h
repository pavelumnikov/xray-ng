// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::threading
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename Type0, typename Type1 >
void 
backoff_while_equals(Type0& location, Type1 value) noexcept
{
    BackoffType backoff {};

    while(location == value)
        backoff.pause();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename Type0, typename Type1 >
void 
backoff_while_not_equals(Type0& location, Type1 value) noexcept
{
    BackoffType backoff {};

    while(location != value)
        backoff.pause();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename BackoffFunction >
void 
backoff_do_while_function(BackoffFunction&& function) noexcept
{
    BackoffType backoff {};

    do
    {
        backoff.pause();
    } while(function());
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename BackoffType, typename BackoffFunction >
void
backoff_while_function(BackoffFunction&& function) noexcept
{
    BackoffType backoff {};

    while(function())
        backoff.pause();
}

} // namespace xr::threading
//-----------------------------------------------------------------------------------------------------------
