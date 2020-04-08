// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/integer_fwd.h"      // self include
#include "corlib/etl/integer_traits.h"   // for integer_traits
#include <limits>
#include <cstdint>

//------------------------------------------------------------------------------
namespace xr::etl
{

//  fast integers from least integers
//  int_fast_t<> works correctly for unsigned too, in spite of the name.
template< typename LeastInt >
struct int_fast_t
{
    typedef LeastInt fast;
    typedef fast     type;
}; // imps may specialize

namespace detail
{

//  convert category to type 
template< int32_t Category > struct int_least_helper
{}; // default is empty
template< int32_t Category > struct uint_least_helper
{}; // default is empty

//  specializatons: 1=long, 2=int, 3=short, 4=signed char,
//     6=unsigned long, 7=unsigned int, 8=unsigned short, 9=unsigned char
//  no specializations for 0 and 5: requests for a type > long are in error
template<> struct int_least_helper<1>
{
    typedef int64_t least;
};
template<> struct int_least_helper<2>
{
    typedef long least;
};
template<> struct int_least_helper<3>
{
    typedef int least;
};
template<> struct int_least_helper<4>
{
    typedef short least;
};
template<> struct int_least_helper<5>
{
    typedef signed char least;
};

template<> struct uint_least_helper<1>
{
    typedef uint64_t least;
};
template<> struct uint_least_helper<2>
{
    typedef unsigned long least;
};
template<> struct uint_least_helper<3>
{
    typedef unsigned int least;
};
template<> struct uint_least_helper<4>
{
    typedef unsigned short least;
};
template<> struct uint_least_helper<5>
{
    typedef unsigned char least;
};

template<int32_t Bits> struct exact_signed_base_helper
{};
template<int32_t Bits> struct exact_unsigned_base_helper
{};

template <> struct exact_signed_base_helper<sizeof(signed char)* CHAR_BIT>
{
    typedef signed char exact;
};
template <> struct exact_unsigned_base_helper<sizeof(unsigned char)* CHAR_BIT>
{
    typedef unsigned char exact;
};
#if USHRT_MAX != UCHAR_MAX
template <> struct exact_signed_base_helper<sizeof(short)* CHAR_BIT>
{
    typedef short exact;
};
template <> struct exact_unsigned_base_helper<sizeof(unsigned short)* CHAR_BIT>
{
    typedef unsigned short exact;
};
#endif
#if UINT_MAX != USHRT_MAX
template <> struct exact_signed_base_helper<sizeof(int)* CHAR_BIT>
{
    typedef int exact;
};
template <> struct exact_unsigned_base_helper<sizeof(unsigned int)* CHAR_BIT>
{
    typedef unsigned int exact;
};
#endif
#if ULONG_MAX != UINT_MAX
template <> struct exact_signed_base_helper<sizeof(long)* CHAR_BIT>
{
    typedef long exact;
};
template <> struct exact_unsigned_base_helper<sizeof(unsigned long)* CHAR_BIT>
{
    typedef unsigned long exact;
};
#endif


} // namespace detail

//  integer templates specifying number of bits  ---------------------------//

//  signed
template< int32_t Bits >   // bits (including sign) required
struct int_t : public detail::exact_signed_base_helper<Bits>
{
    static_assert(Bits <= (int)(sizeof(intmax_t) * CHAR_BIT),
        "No suitable signed integer type with the requested number of bits is available.");
    typedef typename detail::int_least_helper
        <1 +
        (Bits - 1 <= ::std::numeric_limits<long>::digits) +
        (Bits - 1 <= ::std::numeric_limits<int>::digits) +
        (Bits - 1 <= ::std::numeric_limits<short>::digits) +
        (Bits - 1 <= ::std::numeric_limits<signed char>::digits)
        >::least  least;
    typedef typename int_fast_t<least>::type  fast;
};

//  unsigned
template< int Bits >   // bits required
struct uint_t : public detail::exact_unsigned_base_helper<Bits>
{
    static_assert(Bits <= (int)(sizeof(uintmax_t) * CHAR_BIT),
        "No suitable unsigned integer type with the requested number of bits is available.");

    typedef typename detail::uint_least_helper
        < 1 +
        (Bits <= ::std::numeric_limits<unsigned long>::digits) +
        (Bits <= ::std::numeric_limits<unsigned int>::digits) +
        (Bits <= ::std::numeric_limits<unsigned short>::digits) +
        (Bits <= ::std::numeric_limits<unsigned char>::digits)
        >::least  least;

    typedef typename int_fast_t<least>::type  fast;
    // int_fast_t<> works correctly for unsigned too, in spite of the name.
};

//  integer templates specifying extreme value  ----------------------------//

//  signed
template< long long MaxValue >   // maximum value to require support
struct int_max_value_t
{
    typedef typename detail::int_least_helper
        < 1 +
        (MaxValue <= integer_traits<long>::const_max) +
        (MaxValue <= integer_traits<int>::const_max) +
        (MaxValue <= integer_traits<short>::const_max) +
        (MaxValue <= integer_traits<signed char>::const_max)
        >::least  least;
    typedef typename int_fast_t<least>::type  fast;
};

template< long long MinValue >   // minimum value to require support
struct int_min_value_t
{
    typedef typename detail::int_least_helper
        < 1 +
        (MinValue >= integer_traits<long>::const_min) +
        (MinValue >= integer_traits<int>::const_min) +
        (MinValue >= integer_traits<short>::const_min) +
        (MinValue >= integer_traits<signed char>::const_min)
        >::least  least;
    typedef typename int_fast_t<least>::type  fast;
};

//  unsigned
template< unsigned long long MaxValue >   // minimum value to require support
struct uint_value_t
{
    typedef typename detail::uint_least_helper
        < 1 +
        (MaxValue <= integer_traits<unsigned long>::const_max) +
        (MaxValue <= integer_traits<unsigned int>::const_max) +
        (MaxValue <= integer_traits<unsigned short>::const_max) +
        (MaxValue <= integer_traits<unsigned char>::const_max)
        >::least  least;
    typedef typename int_fast_t<least>::type  fast;
};

} // namespace xr::etl
//------------------------------------------------------------------------------