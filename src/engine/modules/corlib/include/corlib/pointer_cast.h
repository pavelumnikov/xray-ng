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

#include "EASTL/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::details::pointer_cast_impl
{

//------------------------------------------------------------------------------

template < typename T0, typename T1, bool IsConst >
struct helper final
{
    static T1 convert(T0* const value)
    {
        auto const temp = static_cast<void*>(value);
        return(static_cast<T1>(temp));
    }
}; // struct helper

//------------------------------------------------------------------------------
template < typename T0, typename T1 >
struct helper<T0, T1, true> final
{
    static T1 convert(T0* const value)
    {
        auto const temp = static_cast<const void*>(value);
        return(static_cast<T1>(temp));
    }
}; // struct helper

} // namespace xr::details::pointer_cast
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr
{

//------------------------------------------------------------------------------
/**
 */
template < typename DestinationType, typename SourceType >
DestinationType pointer_cast(SourceType* const source)
{
    using destination_pointerless_type = eastl::remove_pointer_t<DestinationType>;
    using pure_destination_type = eastl::remove_reference_t<destination_pointerless_type>;
    using pure_source_type = eastl::remove_reference_t<SourceType>;

    static_assert(eastl::is_const_v<pure_source_type> == eastl::is_const_v<pure_destination_type>,
        "types are not const correct");

    static_assert(!eastl::is_base_of_v< pure_source_type, pure_destination_type >,
        "downcast detected! use static_cast checked instead");

    static_assert(!eastl::is_base_of_v< pure_destination_type, pure_source_type >,
        "upcast detected! use implicit conversion instead");

    static_assert(!(eastl::is_class<pure_source_type>::value && 
        eastl::is_class<pure_destination_type>::value),
        "crosscast detected! use dynamic_cast instead");

    return details::pointer_cast_impl::helper<
        pure_source_type, DestinationType,std::is_const_v<pure_source_type>>::convert(source);
}

} // namespace xr
//------------------------------------------------------------------------------