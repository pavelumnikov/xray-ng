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

#include "EASTL/utility.h"

//------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

//------------------------------------------------------------------------------
/**
*/
template <typename T>
constexpr void
cswap(T &a, T &b)
{
    auto tmp = a;
    a = b;
    b = tmp;
}

//------------------------------------------------------------------------------
/**
*/
template <typename T, typename U>
constexpr void
cswap(eastl::pair<T, U> & a, eastl::pair<T, U> & b)
{
    cswap(a.first, b.first);
    cswap(a.second, b.second);
}

//------------------------------------------------------------------------------
/**
*/
template <class... Tys, size_t... Is>
constexpr void
cswap(eastl::tuple<Tys...> &a, eastl::tuple<Tys...> &b, eastl::index_sequence<Is...>)
{
    using swallow = int[];
    (void)swallow
    {
        (cswap(eastl::get<Is>(a), eastl::get<Is>(b)), 0)...
    };
}

//------------------------------------------------------------------------------
/**
*/
template <class... Tys>
constexpr void
cswap(eastl::tuple<Tys...> &a, eastl::tuple<Tys...> &b)
{
    cswap(a, b, eastl::make_index_sequence<sizeof...(Tys)>());
}

} // namespace xr
//------------------------------------------------------------------------------