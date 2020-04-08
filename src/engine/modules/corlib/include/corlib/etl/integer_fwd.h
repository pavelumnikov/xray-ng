// This file is a part of xray-ng engine
//

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