// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/namespaces.h"
#include "corlib/macro/noexcept.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

template< typename Type >
struct default_value;

template<>
struct default_value<int8_t> final
{
    static constexpr auto value = static_cast<int8_t>(0);
};

template<>
struct default_value<uint8_t> final
{
    static constexpr auto value = static_cast<uint8_t>(0);
};

template< typename Type >
constexpr auto default_value_v = default_value< Type >::value;


template< typename Type, Type Input >
struct as_uint32 final
{
    static constexpr auto value = static_cast<uint32_t>(Input);
};

template< typename Type, Type Input >
struct as_int32 final
{
    static constexpr auto value = static_cast<int32_t>(Input);
};

template< typename Type, Type Input >
constexpr auto as_uint32_v = as_uint32<Type, Input>::value;

template< typename Type, Type Input >
constexpr auto as_int32_v = as_int32<Type, Input>::value;

template <typename U, typename T>
struct is_explicitly_convertible final
{
    static constexpr auto value = eastl::is_constructible<T, U>::value &&
        eastl::is_convertible<U, T>::value;
};

template <typename U, typename T>
constexpr auto is_explicitly_convertible_v = is_explicitly_convertible<U, T>::value;

// implicit_cast< >
// I believe this was originally going to be in the C++ standard but 
// was left out by accident. It's even milder than static_cast.
// I use it instead of static_cast<> to emphasize that I'm not doing
// anything nasty. 
// Usage is identical to static_cast<>
template<typename OutputClass, typename InputClass>
OutputClass implicit_cast(InputClass const input)
{
    return input;
}

// unsafe_type_cast< >
// This is truly evil. It completely subverts C++'s type system, allowing you 
// to cast from any class to any other class. Technically, using a union 
// to perform the cast is undefined behaviour (even in C). But we can see if
// it is OK by checking that the union is the same size as each of its members.
// unsafe_type_cast<> should only be used for compiler-specific workarounds. 
// Usage is identical to reinterpret_cast<>.
template<typename OutputClass, typename InputClass>
OutputClass unsafe_type_cast(InputClass const input)
{
    union horrible_union
    {
        OutputClass out;
        InputClass in;
    } u {};

    // Cause a compile-time error if in, out and u are not the same size.
    // If the compile fails here, it means the compiler has peculiar
    // unions which would prevent the cast from working.
    static_assert(sizeof(InputClass) == sizeof(u)
        && sizeof(InputClass) == sizeof(OutputClass));

    u.in = input;
    return u.out;
}

template <typename E>
constexpr typename eastl::underlying_type<E>::type to_underlying(E e) XR_NOEXCEPT
{
    return static_cast<typename eastl::underlying_type<E>::type>(e);
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
