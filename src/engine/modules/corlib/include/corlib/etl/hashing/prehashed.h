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

#include "corlib/etl/hashing/elsa_hash.h"
#include "EASTL/internal/copy_help.h"

//------------------------------------------------------------------------------
namespace eastl
{
    template<typename T> struct hash;
} // namespace eastl
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::etl::hashing
{

//------------------------------------------------------------------------------
template<typename Type>
class pre_hashed final
{
public:
    using type = Type;
    using reference = eastl::add_lvalue_reference_t<Type>;
    using const_reference = eastl::add_lvalue_reference_t<eastl::add_const_t<Type>>;

    explicit constexpr pre_hashed(type const& data);

    friend constexpr bool operator == (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs);
    friend constexpr bool operator != (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs);

    friend constexpr bool operator == (const_reference lhs, pre_hashed<Type> const& rhs);
    friend constexpr bool operator != (const_reference lhs, pre_hashed<Type> const& rhs);

    friend constexpr bool operator == (pre_hashed<Type> const& lhs, const_reference rhs);
    friend constexpr bool operator != (pre_hashed<Type> const& lhs, const_reference rhs);

    constexpr reference data() const;
    constexpr size_t hash() const;

private:
    type m_data;
    size_t m_hash;
}; // class pre_hashed<Type>

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr pre_hashed<Type>::pre_hashed(type const& data)
    : m_data { data }
    , m_hash { hashing::elsa<Type>{}(m_data) }
{}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator == (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs)
{
    return lhs.m_hash == rhs.m_hash;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator != (pre_hashed<Type> const& lhs, pre_hashed<Type> const& rhs)
{
    return lhs.m_hash == rhs.m_hash;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator == (typename pre_hashed<Type>::const_reference lhs,
    pre_hashed<Type> const& rhs)
{
    auto hash = hashing::elsa<Type>()(lhs);
    return hash == rhs.m_hash;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator != (typename pre_hashed<Type>::const_reference lhs,
    pre_hashed<Type> const& rhs)
{
    auto hash = hashing::elsa<Type>()(lhs);
    return hash != rhs.m_hash;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator == (pre_hashed<Type> const& lhs,
    typename pre_hashed<Type>::const_reference rhs)
{
    auto hash = hashing::elsa<Type>()(rhs);
    return hash == lhs.m_hash;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr bool operator != (pre_hashed<Type> const& lhs,
    typename pre_hashed<Type>::const_reference rhs)
{
    auto hash = hashing::elsa<Type>()(rhs);
    return hash == lhs.m_hash;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr pre_hashed<Type>::reference
pre_hashed<Type>::data() const
{
    return m_data;
}

//------------------------------------------------------------------------------
/**
 */
template<typename Type>
constexpr size_t pre_hashed<Type>::hash() const
{
    return m_hash;
}

} // namespace xr::etl::hashing
//------------------------------------------------------------------------------