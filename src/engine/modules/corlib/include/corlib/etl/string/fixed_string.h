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

#include "corlib/etl/hashing/std/string_hash.h"
#include "EASTL/fixed_string.h"

//------------------------------------------------------------------------------
namespace xr::etl::string
{

//------------------------------------------------------------------------------
template<int Count>
using fixed_string = eastl::fixed_string<char, Count>;

//------------------------------------------------------------------------------
template<int Count>
using fixed_wstring = eastl::fixed_string<wchar_t, Count>;

//------------------------------------------------------------------------------
using fixed_string32 = fixed_string<32>;
using fixed_wstring32 = fixed_wstring<32>;

//------------------------------------------------------------------------------
using fixed_string64 = fixed_string<64>;
using fixed_wstring64 = fixed_wstring<64>;

//------------------------------------------------------------------------------
using fixed_string128 = fixed_string<128>;
using fixed_wstring128 = fixed_wstring<128>;

//------------------------------------------------------------------------------
using fixed_string256 = fixed_string<256>;
using fixed_wstring256 = fixed_wstring<256>;

//------------------------------------------------------------------------------
using fixed_string512 = fixed_string<512>;
using fixed_wstring512 = fixed_wstring<512>;

//------------------------------------------------------------------------------
/**
*/
template<int Count>
eastl::string_view to_string_view(fixed_string<Count> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

//------------------------------------------------------------------------------
/**
*/
template<int Count>
eastl::wstring_view to_wstring_view(fixed_wstring<Count> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

} // namespace xr::etl::string
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace eastl
{

//------------------------------------------------------------------------------
template<int N>
struct hash<xr::etl::string::fixed_string<N>>
{
    size_t operator()(xr::etl::string::fixed_string<N> const& s) const;
}; // struct hash<az::strings::fixed_string<N>>

//------------------------------------------------------------------------------
/**
*/
template<int N>
size_t hash<xr::etl::string::fixed_string<N>>::operator()
(xr::etl::string::fixed_string<N> const& s) const
{
    return xr::etl::hashing::std::string_hash(s);
}


//------------------------------------------------------------------------------
template<size_t N>
struct hash<xr::etl::string::fixed_wstring<N>>
{
    size_t operator()(xr::etl::string::fixed_wstring<N> const& s) const;
}; // struct hash<az::strings::fixed_wstring<N>>

//------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t hash<xr::etl::string::fixed_wstring<N>>::operator()
(xr::etl::string::fixed_wstring<N> const& s) const
{
    return xr::etl::hashing::std::string_hash(s);
}

} // namespace xr::etl::string
//------------------------------------------------------------------------------