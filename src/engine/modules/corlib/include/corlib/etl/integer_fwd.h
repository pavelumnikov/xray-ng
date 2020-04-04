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

#include <limits.h>  // for UCHAR_MAX, etc.
#include <stddef.h>  // for std::size_t
#include "EASTL/numeric_limits.h"

namespace xr::etl
{

typedef uintmax_t static_min_max_unsigned_type;
typedef intmax_t  static_min_max_signed_type;
typedef uintmax_t static_log2_argument_type;
typedef int static_log2_result_type;

template < class T >
class integer_traits;

template <  >
class integer_traits< bool >;

template <  >
class integer_traits< char >;

template <  >
class integer_traits< signed char >;

template <  >
class integer_traits< unsigned char >;

template <  >
class integer_traits< wchar_t >;

template <  >
class integer_traits< short >;

template <  >
class integer_traits< unsigned short >;

template <  >
class integer_traits< int >;

template <  >
class integer_traits< unsigned int >;

template <  >
class integer_traits< long >;

template <  >
class integer_traits< unsigned long >;

template <  >
class integer_traits<int64_t>;

template <  >
class integer_traits<uint64_t>;

template < typename LeastInt >
struct int_fast_t;

template< int Bits >
struct int_t;

template< int Bits >
struct uint_t;

template< long long MaxValue >   // maximum value to require support
struct int_max_value_t;

template< long long MinValue >   // minimum value to require support
struct int_min_value_t;

template< unsigned long long MaxValue >   // maximum value to require support
struct uint_value_t;

template<size_t Bit>
struct high_bit_mask_t;

template<size_t Bits>
struct low_bits_mask_t;

template <>
struct low_bits_mask_t<eastl::numeric_limits<unsigned char>::digits>;

template <static_log2_argument_type Value >
struct static_log2;

template <> struct static_log2<0u>;

template <static_min_max_signed_type Value1, static_min_max_signed_type Value2>
struct static_signed_min;

template <static_min_max_signed_type Value1, static_min_max_signed_type Value2>
struct static_signed_max;

template <static_min_max_unsigned_type Value1, static_min_max_unsigned_type Value2>
struct static_unsigned_min;

template <static_min_max_unsigned_type Value1, static_min_max_unsigned_type Value2>
struct static_unsigned_max;

}  // namespace xr::etl
//------------------------------------------------------------------------------