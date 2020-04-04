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

#include "corlib/etl/algorithms/aligning.h"

//------------------------------------------------------------------------------
namespace xr::etl::algorithms::details
{

//------------------------------------------------------------------------------
template <typename T, typename Compare>
struct lower_bound_impl final
{
    T const &value_;
    Compare const &compare_;

    constexpr lower_bound_impl(T const &value, Compare const &compare)
        : value_(value)
        , compare_(compare)
    {}

    template <class ForwardIt>
    constexpr ForwardIt doit_fast(ForwardIt first, eastl::integral_constant<size_t, 0>)
    {
        return first;
    }

    template <class ForwardIt, size_t N>
    constexpr ForwardIt doit_fast(ForwardIt first,
        eastl::integral_constant<size_t, N>)
    {
        auto constexpr step = N / 2;
        static_assert(N / 2 == N - N / 2 - 1, "power of two minus 1");
        auto it = first + step;
        auto next_it = compare_(*it, value_) ? it + 1 : first;
        return doit_fast(next_it, ::std::integral_constant<size_t, N / 2>{});
    }

    template <class ForwardIt, size_t N>
    constexpr ForwardIt doitfirst(ForwardIt first,
        eastl::integral_constant<size_t, N>, ::std::integral_constant<bool, true>)
    {
        return doit_fast(first, ::std::integral_constant<size_t, N>{});
    }

    template <class ForwardIt, std::size_t N>
    constexpr ForwardIt doitfirst(ForwardIt first,
        eastl::integral_constant<size_t, N>, ::std::integral_constant<bool, false>)
    {
        auto constexpr next_power = next_highest_power_of_two(N);
        auto constexpr next_start = next_power / 2 - 1;
        auto it = first + next_start;

        if(compare_(*it, value_))
        {
            auto constexpr next = N - next_start - 1;
            return doitfirst(it + 1, eastl::integral_constant<size_t, next>{},
                eastl::integral_constant<bool, next_highest_power_of_two(next) - 1 == next>{});
        }
        
        return doit_fast(first, ::std::integral_constant<size_t, next_start>{});
    }

    template <class ForwardIt>
    constexpr ForwardIt doitfirst(ForwardIt first,
        eastl::integral_constant<size_t, 1>, ::std::integral_constant<bool, false>)
    {
        return doit_fast(first, ::std::integral_constant<size_t, 1>{});
    }
};
} // namespace xr::etl::algorithms::details


//------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

template <size_t N, class ForwardIt, typename T, typename Compare>
constexpr ForwardIt 
lower_bound(ForwardIt first, const T &value, Compare const &compare)
{
    return details::lower_bound_impl<T, Compare> {value, compare}
        .doitfirst(first, ::std::integral_constant<size_t, N>{},
        ::std::integral_constant<bool, next_highest_power_of_two(N) - 1 == N>{});
}

} // namespace xr::etl::algorithms
//------------------------------------------------------------------------------