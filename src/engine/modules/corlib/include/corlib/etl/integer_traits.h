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

#include <stdint.h>
#include "EASTL/numeric_limits.h"

//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
namespace detail
{
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

template<class T> class integer_traits;

template<>
class integer_traits<bool> : public detail::integer_traits_base<bool>
{};

template<>
class integer_traits<char> : public detail::integer_traits_base<char>
{};

template<>
class integer_traits<signed char> : public detail::integer_traits_base<signed char>
{};

template<>
class integer_traits<unsigned char> : public detail::integer_traits_base<unsigned char>
{};

template<>
class integer_traits<wchar_t> : public detail::integer_traits_base<wchar_t>
{};

template<>
class integer_traits<short> : public detail::integer_traits_base<short>
{};

template<>
class integer_traits<unsigned short> : public detail::integer_traits_base<unsigned short>
{};

template<>
class integer_traits<int> : public detail::integer_traits_base<int>
{};

template<>
class integer_traits<unsigned int> : public detail::integer_traits_base<unsigned int>
{};

template<>
class integer_traits<long> : public detail::integer_traits_base<long>
{};

template<>
class integer_traits<unsigned long> : public detail::integer_traits_base<unsigned long>
{};

template<>
class integer_traits<int64_t> : public detail::integer_traits_base<int64_t>
{};

template<>
class integer_traits<uint64_t> : public detail::integer_traits_base<uint64_t>
{};

} // namespace namespace xr
//------------------------------------------------------------------------------