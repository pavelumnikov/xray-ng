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

#include "EASTL/fixed_string.h"

//------------------------------------------------------------------------------
namespace xr::etl::hashing::std
{

//------------------------------------------------------------------------------
/**
*/
template<typename TString>
size_t string_hash(TString const& s)
{
    auto constexpr init =
        size_t(sizeof(size_t) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5);

    auto constexpr multiplier =
        size_t(sizeof(size_t) == 8 ? 0x100000001b3 : 0x1000193);

    auto hash = init;
    auto const end = eastl::end(s);

    for(auto current = eastl::begin(s); current != end; ++current)
    {
        hash ^= *current;
        hash *= multiplier;
    }

    return hash;
}

} // namespace xr::etl::hashing::std
//------------------------------------------------------------------------------