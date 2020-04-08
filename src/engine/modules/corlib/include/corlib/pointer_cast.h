// This file is a part of xray-ng engine
//

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