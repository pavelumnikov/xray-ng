// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/namespaces.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details, pointer_cast)

//-----------------------------------------------------------------------------------------------------------

template < typename T0, typename T1, bool IsConst >
struct helper final
{
    static T1 convert(T0* value)
    {
        pvoid temp = static_cast<pvoid>(value);
        return(static_cast<T1>(temp));
    }
}; // struct helper

//-----------------------------------------------------------------------------------------------------------
template < typename T0, typename T1 >
struct helper<T0, T1, true> final
{
    static T1 convert(T0* const value)
    {
        pcvoid temp = static_cast<pcvoid>(value);
        return(static_cast<T1>(temp));
    }
}; // struct helper

XR_NAMESPACE_END(xr, utils, details, pointer_cast)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename DestinationType, typename SourceType>
inline DestinationType pointer_cast(SourceType* const source)
{
    using destination_pointerless_type = eastl::remove_pointer_t<DestinationType>;
    using pure_destination_type = eastl::remove_reference_t<destination_pointerless_type>;
    using pure_source_type = eastl::remove_reference_t<SourceType>;

    XR_STATIC_ASSERT(eastl::is_const<pure_source_type>::value == eastl::is_const<pure_destination_type>::value,
        "types are not const correct");

    static_assert(!eastl::is_base_of< pure_source_type, pure_destination_type >::value,
        "downcast detected! use static_cast checked instead");

    static_assert(!eastl::is_base_of< pure_destination_type, pure_source_type >::value,
        "upcast detected! use implicit conversion instead");

    static_assert(!(eastl::is_class<pure_source_type>::value && eastl::is_class<pure_destination_type>::value),
        "crosscast detected! use dynamic_cast instead");

    return details::pointer_cast::helper<
        pure_source_type, DestinationType, eastl::is_const<pure_source_type>::value>::convert(source);
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
