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

#include "corlib/platform.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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
//------------------------------------------------------------------------------