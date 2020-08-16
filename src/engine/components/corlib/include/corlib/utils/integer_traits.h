// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/numeric_limits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
namespace detail
{

//-----------------------------------------------------------------------------------------------------------
template<typename TraitsType>
class integer_traits_base : public eastl::numeric_limits<TraitsType>
{
    using super = eastl::numeric_limits<TraitsType>;

public:
    using value_type = TraitsType;

    static constexpr bool is_integral = eastl::is_integral_v<value_type>;
    static constexpr value_type const_min = super::min();
    static constexpr value_type const_max = super::max();
};

//  A definition is required even for integral static constants
template<typename TraitsType>
const bool integer_traits_base<TraitsType>::is_integral;

template<typename TraitsType>
const integer_traits_base<TraitsType>::value_type
integer_traits_base<TraitsType>::const_min;

template<typename TraitsType>
const integer_traits_base<TraitsType>::value_type
integer_traits_base<TraitsType>::const_max;

} // namespace detail

//-----------------------------------------------------------------------------------------------------------
template<class T> class integer_traits;

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<bool> : public detail::integer_traits_base<bool>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<char> : public detail::integer_traits_base<char>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<signed char> : public detail::integer_traits_base<signed char>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<unsigned char> : public detail::integer_traits_base<unsigned char>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<wchar_t> : public detail::integer_traits_base<wchar_t>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<short> : public detail::integer_traits_base<short>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<unsigned short> : public detail::integer_traits_base<unsigned short>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<int> : public detail::integer_traits_base<int>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<unsigned int> : public detail::integer_traits_base<unsigned int>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<long> : public detail::integer_traits_base<long>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<unsigned long> : public detail::integer_traits_base<unsigned long>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<int64_t> : public detail::integer_traits_base<int64_t>
{};

//-----------------------------------------------------------------------------------------------------------
template<>
class integer_traits<uint64_t> : public detail::integer_traits_base<uint64_t>
{};

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
