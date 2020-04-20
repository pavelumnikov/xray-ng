// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"
#include "EASTL/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
// Helper structure to convert unsigned types for templates into signed when using
// decrement
template< typename T >
struct signed_unsigned_inversion final
{
	typedef T type;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint8_t> final
{
	using type = int8_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint16_t> final
{
	using type = int16_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint32_t> final
{
	using type = int32_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint64_t> final
{
	using type = int64_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int8_t> final
{
	using type = uint8_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int16_t> final
{
	using type = uint16_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int32_t> final
{
	using type = uint32_t;
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int64_t> final
{
	using type = uint64_t;
};

//-----------------------------------------------------------------------------------------------------------
template< typename Type >
using signed_unsigned_inversion_t = typename signed_unsigned_inversion<Type>::type;

//-----------------------------------------------------------------------------------------------------------
// number | twos complement     | ones complement     | sign/magnitude
// == == =|= == == == == == == == == == == == == == ==|= == == == == == == =
//	-1     | 1111 1111 1111 1111 | 1111 1111 1111 1110 | 1000 0000 0000 0001
template< typename Type >
struct is_signed_unsigned_complement_system final
{
    using unsigned_type = Type;
    using signed_type = typename signed_unsigned_inversion<Type>::Type;

    static const auto value = (static_cast<signed_type>(~(unsigned_type())) == static_cast<signed_type>(-1));
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename Type >
constexpr auto is_signed_unsigned_complement_system_v = is_signed_unsigned_complement_system<Type>::value;

//-----------------------------------------------------------------------------------------------------------
template<typename Type>
struct invert_if_unsigned final
{
    using type = eastl::conditional_t<eastl::is_unsigned_v<Type>, signed_unsigned_inversion_t<Type>, Type>;
};

//-----------------------------------------------------------------------------------------------------------
template<typename Type>
using invert_if_unsigned_t = typename invert_if_unsigned<Type>::type;

//-----------------------------------------------------------------------------------------------------------
template<typename Type>
struct invert_if_signed final
{
    using type = eastl::conditional_t<eastl::is_signed_v<Type>, signed_unsigned_inversion_t<Type>, Type>;
};

//-----------------------------------------------------------------------------------------------------------
template<typename Type>
using invert_if_signed_t = typename invert_if_signed<Type>::type;

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline unsigned int 
flip_sign(unsigned int const x)
{
    auto constexpr sign_bit = ~0 ^ (~0 >> 1);
    return x ^ sign_bit;
}


} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
