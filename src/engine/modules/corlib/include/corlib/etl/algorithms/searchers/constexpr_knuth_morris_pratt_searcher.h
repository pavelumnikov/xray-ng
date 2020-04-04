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

#include "corlib/etl/containers/constexpr_array.h"
#include "corlib/type_traits.h"

//------------------------------------------------------------------------------
namespace xr::etl::algorithms::searchers
{

// text book implementation from
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm

template<typename Type, size_t size> 
class constexpr_knuth_morris_pratt_searcher
{
    static_assert(std::is_string_v<Type>, 
        "Input type must be a string-compatible type");

    containers::constexpr_array<ptrdiff_t, size> step_;
    containers::constexpr_array<Type, size> needle_;

    static constexpr containers::constexpr_array<ptrdiff_t, size>
        build_kmp_cache(Type const (&needle)[size + 1])
    {
        ptrdiff_t cnd = 0;
        containers::constexpr_array<ptrdiff_t, size> cache;

        cache.fill(-1);
        for(size_t pos = 1; pos < size; ++pos)
        {
            if(needle[pos] == needle[cnd])
            {
                cache[pos] = cache[cnd];
                cnd += 1;
            }
            else
            {
                cache[pos] = cnd;
                cnd = cache[cnd];

                while(cnd >= 0 && needle[pos] != needle[cnd])
                {
                    cnd = cache[cnd];
                }

                cnd += 1;
            }
        }
        return cache;
    }

public:
    constexpr constexpr_knuth_morris_pratt_searcher(Type const (&needle)[size + 1])
        : step_ { build_kmp_cache(needle) }, needle_(needle) {}

    template <class ForwardIterator>
    constexpr ::eastl::pair<ForwardIterator, ForwardIterator> operator()
        (ForwardIterator first, ForwardIterator last) const
    {
        size_t i = 0;
        ForwardIterator iter = first;
        while(iter != last)
        {
            if(needle_[i] == *iter)
            {
                if(i == (size - 1))
                {
                    return { iter - i, iter - i + size };
                }

                ++i;
                ++iter;
            }
            else
            {
                if(step_[i] > -1)
                {
                    i = step_[i];
                }
                else
                {
                    ++iter;
                    i = 0;
                }
            }
        }
        return { last, last };
    }
};

template<typename Type, size_t N>
constexpr constexpr_knuth_morris_pratt_searcher<Type, N - 1> make_knuth_morris_pratt_searcher(Type const (&needle)[N])
{
    return { needle };
}

} // namespace xr::etl::algorithms::searchers
//------------------------------------------------------------------------------