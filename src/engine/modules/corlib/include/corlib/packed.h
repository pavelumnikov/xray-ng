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

#include "corlib/etl/type_traits.h"
#include <intrin.h>

//------------------------------------------------------------------------------
namespace xr
{

//------------------------------------------------------------------------------
union packed_2x8bits_to_16bit final
{
    constexpr packed_2x8bits_to_16bit(int16_t const val) : m_16bit(val)
    {}

    constexpr packed_2x8bits_to_16bit(int8_t const upper, uint8_t const lower)
        : m_16bit(lower | (upper << 15))
    {}

    int16_t m_16bit;
    struct
    {
        int8_t upper;
        uint8_t lower;
    } m_8bits;
}; // union packed_2x8bits_to_16bit

//------------------------------------------------------------------------------
union packed_2x8bits_to_16bit_unsigned final
{
    explicit constexpr packed_2x8bits_to_16bit_unsigned(uint16_t const val) : m_16bit(val)
    {}
    constexpr packed_2x8bits_to_16bit_unsigned(uint8_t const upper, uint8_t const lower)
        : m_16bit(lower | (upper << 15))
    {}

    uint16_t m_16bit;
    struct
    {
        uint8_t upper;
        uint8_t lower;
    } m_8bits;
}; // union packed_2x8bits_to_16bit


//------------------------------------------------------------------------------
union packed_int32_float final
{
    explicit constexpr packed_int32_float(const float Value) : m_float(Value)
    {}

    explicit constexpr packed_int32_float(const int32_t Value) : m_int32(Value)
    {}

    float m_float;
    int32_t m_int32;
}; // union packed_int32_float

//------------------------------------------------------------------------------
union packed_uint32_float final
{
    explicit constexpr packed_uint32_float(const float value) : m_float(value)
    {}

    explicit constexpr packed_uint32_float(const uint32_t value) : m_uint32(value)
    {}

    float m_float;
    uint32_t m_uint32;
}; // union packed_uint32_float

//------------------------------------------------------------------------------
union packed_2x32bits_to_64bit final
{
    explicit constexpr packed_2x32bits_to_64bit(int64_t const val) : m_64bit(val)
    {}
    constexpr packed_2x32bits_to_64bit(int32_t const upper, uint32_t const lower) 
        : m_64bit(lower | (upper << 31))
    {}

    int64_t m_64bit;
    struct
    {
        int32_t upper;
        uint32_t lower;
    } m_32bits;
}; // union packed_2x32bits_to_64bit

//------------------------------------------------------------------------------
union packed_2x32bits_to_64bit_unsigned final
{
    explicit constexpr packed_2x32bits_to_64bit_unsigned(uint64_t const val) : m_64bit(val)
    {}

    constexpr packed_2x32bits_to_64bit_unsigned(uint32_t const upper, uint32_t const lower) 
        : m_64bit(lower | (upper << 31))
    {}

    uint64_t m_64bit;
    struct
    {
        uint32_t upper;
        uint32_t lower;
    } m_32bits;
}; // union packed_2x32bits_to_64bit

//------------------------------------------------------------------------------
template<typename T>
union packed_struct_max_uint64 final
{
    explicit constexpr packed_struct_max_uint64(uint64_t const val) : m_64bit(val)
    {}

    explicit constexpr packed_struct_max_uint64(T v) : m_packed(v)
    {}

    T m_packed;
    uint64_t m_64bit;
}; // union packed_struct_max_uint64

//------------------------------------------------------------------------------
union packed_uint64_double final
{
    explicit constexpr packed_uint64_double(uint64_t const value) : m_uint64(value)
    {}

    explicit constexpr packed_uint64_double(double const value) : m_double(value)
    {}

    uint64_t m_uint64;
    double m_double;
}; // union packed_uint64_double

//------------------------------------------------------------------------------
union packed_int64_double final
{
    explicit constexpr packed_int64_double(int64_t value) : m_int64(value)
    {}

    explicit constexpr packed_int64_double(double value) : m_double(value)
    {}

    int64_t m_int64;
    double m_double;
}; // union packed_int64_double

typedef uint16_t packed_half_vec;

#if XRAY_PLATFORM_WINDOWS

//------------------------------------------------------------------------------
template<typename TType, typename TStorage, size_t Count>
struct packed_vecn_storage final
{
    using value_type = TType;
    using array_type = value_type[Count];
    using storage_type = TStorage;
    using pointer = eastl::add_pointer_t<TType>;
    using const_pointer = eastl::add_const_t<pointer>;

    static_assert((sizeof(value_type)*Count) <= sizeof(storage_type),
        "Array of value_type is higher than aligned storage");

    union
    {
        array_type t;
        storage_type f;
    };

    explicit operator storage_type() const
    {
        return this->f;
    }

    explicit operator const_pointer () const
    {
        return this->t;
    }
}; // struct packed_vecn_storage

//------------------------------------------------------------------------------
/**
*/
using packed_vec8i_128f = packed_vecn_storage<int8_t, __m128, 16>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec8u_128f = packed_vecn_storage<uint8_t, __m128, 16>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec16i_128f = packed_vecn_storage<int16_t, __m128, 8>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec16u_128f = packed_vecn_storage<uint16_t, __m128, 8>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec32i_128f = packed_vecn_storage<int32_t, __m128, 4>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec32u_128f = packed_vecn_storage<uint32_t, __m128, 4>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec64i_128f = packed_vecn_storage<int64_t, __m128, 2>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec64u_128f = packed_vecn_storage<uint64_t, __m128, 2>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec8i_128i = packed_vecn_storage<int8_t, __m128i, 16>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec8u_128i = packed_vecn_storage<uint8_t, __m128i, 16>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec16i_128i = packed_vecn_storage<int16_t, __m128i, 8>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec16u_128i = packed_vecn_storage<uint16_t, __m128i, 8>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec32i_128i = packed_vecn_storage<int32_t, __m128i, 4>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec32u_128i = packed_vecn_storage<uint32_t, __m128i, 4>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec64i_128i = packed_vecn_storage<int64_t, __m128i, 2>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec64u_128i = packed_vecn_storage<uint64_t, __m128i, 2>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec32f_128f = packed_vecn_storage<float, __m128, 4>;

//------------------------------------------------------------------------------
/**
*/
using packed_vec64f_128d = packed_vecn_storage<double, __m128d, 2>;

#endif // #if XRAY_PLATFORM_WINDOWS

} // namespace xr
//------------------------------------------------------------------------------