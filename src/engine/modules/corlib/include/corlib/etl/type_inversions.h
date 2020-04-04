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
#include "EASTL/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
// Helper structure to convert unsigned types for templates into signed when using
// decrement
template< typename T >
struct signed_unsigned_inversion final
{
	typedef T type;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint8_t> final
{
	using type = int8_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint16_t> final
{
	using type = int16_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint32_t> final
{
	using type = int32_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<uint64_t> final
{
	using type = int64_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int8_t> final
{
	using type = uint8_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int16_t> final
{
	using type = uint16_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int32_t> final
{
	using type = uint32_t;
};

//------------------------------------------------------------------------------
template<>
struct signed_unsigned_inversion<int64_t> final
{
	using type = uint64_t;
};

//------------------------------------------------------------------------------
template< typename Type >
using signed_unsigned_inversion_t = typename signed_unsigned_inversion<Type>::type;

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/**
*/
template< typename Type >
constexpr auto is_signed_unsigned_complement_system_v = is_signed_unsigned_complement_system<Type>::value;

//------------------------------------------------------------------------------
template<typename Type>
struct invert_if_unsigned final
{
    using type = eastl::conditional_t<eastl::is_unsigned_v<Type>, signed_unsigned_inversion_t<Type>, Type>;
};

//------------------------------------------------------------------------------
template<typename Type>
using invert_if_unsigned_t = typename invert_if_unsigned<Type>::type;

//------------------------------------------------------------------------------
template<typename Type>
struct invert_if_signed final
{
    using type = eastl::conditional_t<eastl::is_signed_v<Type>, signed_unsigned_inversion_t<Type>, Type>;
};

//------------------------------------------------------------------------------
template<typename Type>
using invert_if_signed_t = typename invert_if_signed<Type>::type;

//------------------------------------------------------------------------------
/**
*/
inline unsigned int 
flip_sign(unsigned int const x)
{
    auto constexpr sign_bit = ~0 ^ (~0 >> 1);
    return x ^ sign_bit;
}


} // namespace xr::etl
//------------------------------------------------------------------------------