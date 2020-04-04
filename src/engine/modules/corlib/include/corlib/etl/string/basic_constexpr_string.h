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

#include "corlib/types.h"
#include "EASTL/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::etl::string
{

//------------------------------------------------------------------------------
template<typename T, size_t N>
class basic_constexpr_string final
{
public:
    using this_type = basic_constexpr_string;
    using value_type = T;
    using pointer = eastl::add_pointer_t<T>;
    using const_pointer = eastl::add_pointer_t<eastl::add_const_t<T>>;
    using reference = eastl::add_lvalue_reference_t<T>;
    using const_reference = eastl::add_lvalue_reference_t<eastl::add_const_t<T>>;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using size_type = size_t;
    using difference_type = eastl::make_signed<size_type>::type;

    static constexpr size_type npos = size_type(-1);

    constexpr basic_constexpr_string() = default;

    template<size_t M>
    explicit constexpr basic_constexpr_string(value_type const(&str)[M]) noexcept;

    constexpr basic_constexpr_string(this_type const& rhs) noexcept = default;
    constexpr basic_constexpr_string(this_type&& rhs) noexcept = default;

    constexpr this_type& operator=(this_type const& rhs) noexcept = default;
    constexpr this_type& operator=(this_type&& rhs) noexcept = default;

    constexpr size_type size() const noexcept;

    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;

    constexpr const_pointer c_str() const noexcept;

private:
    value_type m_str[N] {};
}; // class basic_constexpr_string

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
template<size_t M>
constexpr 
basic_constexpr_string<T, N>::basic_constexpr_string(value_type const(&str)[M]) noexcept
{
    static_assert(M <= N, "input string");
    for(size_t i = 0; i < M; ++i)
        m_str[i] = str[i];
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::const_pointer
basic_constexpr_string<T, N>::c_str() const noexcept
{
    return m_str;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::size_type
basic_constexpr_string<T, N>::size() const noexcept
{
    return N;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::const_iterator 
basic_constexpr_string<T, N>::begin() const noexcept
{
    return m_str;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr typename basic_constexpr_string<T, N>::const_iterator 
basic_constexpr_string<T, N>::end() const noexcept
{
    return (m_str + N);
}

} // namespace xr::etl::string
//------------------------------------------------------------------------------