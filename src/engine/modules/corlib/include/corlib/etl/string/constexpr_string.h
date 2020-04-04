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

#include "corlib/etl/string/basic_constexpr_string.h"
#include "corlib/etl/hashing/elsa_hash.h"
#include "EASTL/string_view.h"

//------------------------------------------------------------------------------
namespace xr::etl::string
{

//------------------------------------------------------------------------------
template<size_t N>
using constexpr_string = basic_constexpr_string<char, N>;

//------------------------------------------------------------------------------
template<size_t N>
using constexpr_wstring = basic_constexpr_string<wchar_t, N>;

//------------------------------------------------------------------------------

using constexpr_string32 = constexpr_string<32>;
using constexpr_wstring32 = constexpr_string<32>;

//------------------------------------------------------------------------------

using constexpr_string64 = constexpr_string<64>;
using constexpr_wstring64 = constexpr_wstring<64>;

//------------------------------------------------------------------------------

using constexpr_string128 = constexpr_string<128>;
using constexpr_wstring128 = constexpr_wstring<128>;

//------------------------------------------------------------------------------

using constexpr_string256 = constexpr_string<256>;
using constexpr_wstring256 = constexpr_wstring<256>;

//------------------------------------------------------------------------------

using constexpr_string512 = constexpr_string<512>;
using constexpr_wstring512 = constexpr_wstring<512>;

//------------------------------------------------------------------------------
/**
*/
template<size_t N>
eastl::string_view to_string_view(constexpr_string<N> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

//------------------------------------------------------------------------------
/**
*/
template<size_t N>
eastl::wstring_view to_wstring_view(constexpr_wstring<N> const& s) noexcept
{
    return { eastl::begin(s), eastl::size(s) };
}

} // namespace xr::etl::string
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
namespace eastl
{

//------------------------------------------------------------------------------
template<size_t N> 
struct hash<xr::etl::string::constexpr_string<N>> final
{
    size_t operator()(xr::etl::string::constexpr_string<N>& s) const;
};

//------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t hash<xr::etl::string::constexpr_string<N>>::operator()
    (xr::etl::string::constexpr_string<N>& s) const
{
    return xr::etl::hasing::elsa<
        az::strings::constexpr_string<N>>{}(s);
}


//------------------------------------------------------------------------------
template<size_t N>
struct hash<xr::etl::string::constexpr_wstring<N>> final
{
    size_t operator()(xr::etl::string::constexpr_wstring<N>& s) const;
};

//------------------------------------------------------------------------------
/**
*/
template<size_t N>
size_t 
hash<xr::etl::string::constexpr_wstring<N>>::operator()
    (xr::etl::string::constexpr_wstring<N>& s) const
{
    return xr::etl::hashing::elsa<
        xr::etl::string::constexpr_wstring<N>>{}(s);
}

} // namespace std
//------------------------------------------------------------------------------