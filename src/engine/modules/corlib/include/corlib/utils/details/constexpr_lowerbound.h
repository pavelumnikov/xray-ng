// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/aligning.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
template <typename T, typename Compare>
struct lower_bound_impl final
{
    T const &value_;
    Compare const &compare_;

    XR_CONSTEXPR_CPP14_OR_INLINE lower_bound_impl(T const &value, Compare const &compare)
        : value_(value)
        , compare_(compare)
    {}

    template <class ForwardIt>
    XR_CONSTEXPR_CPP14_OR_INLINE ForwardIt doit_fast(ForwardIt first, eastl::integral_constant<size_t, 0>)
    {
        return first;
    }

    template <class ForwardIt, size_t N>
    XR_CONSTEXPR_CPP14_OR_INLINE ForwardIt doit_fast(ForwardIt first,
        eastl::integral_constant<size_t, N>)
    {
        size_t XR_CONSTEXPR_CPP14_OR_INLINE step = N / 2;
        XR_STATIC_ASSERT(N / 2 == N - N / 2 - 1, "power of two minus 1");
        ForwardIt it = first + step;
        ForwardIt next_it = compare_(*it, value_) ? it + 1 : first;
        return doit_fast(next_it, std::integral_constant<size_t, N / 2>{});
    }

    template <class ForwardIt, size_t N>
    XR_CONSTEXPR_CPP14_OR_INLINE ForwardIt doitfirst(ForwardIt first,
        eastl::integral_constant<size_t, N>, std::integral_constant<bool, true>)
    {
        return doit_fast(first, std::integral_constant<size_t, N>{});
    }

    template <class ForwardIt, size_t N>
    XR_CONSTEXPR_CPP14_OR_INLINE ForwardIt doitfirst(ForwardIt first,
        eastl::integral_constant<size_t, N>, std::integral_constant<bool, false>)
    {
        size_t XR_CONSTEXPR_CPP14_OR_CONST next_power = next_highest_power_of_two(N);
        size_t XR_CONSTEXPR_CPP14_OR_CONST next_start = next_power / 2 - 1;
        ForwardIt it = first + next_start;

        if(compare_(*it, value_))
        {
            size_t XR_CONSTEXPR_CPP14_OR_CONST next = N - next_start - 1;
            return doitfirst(it + 1, eastl::integral_constant<size_t, next>{},
                eastl::integral_constant<bool, next_highest_power_of_two(next) - 1 == next>{});
        }
        
        return doit_fast(first, std::integral_constant<size_t, next_start>{});
    }

    template <class ForwardIt>
    XR_CONSTEXPR_CPP14_OR_INLINE ForwardIt doitfirst(ForwardIt first,
        eastl::integral_constant<size_t, 1>, ::std::integral_constant<bool, false>)
    {
        return doit_fast(first, std::integral_constant<size_t, 1>{});
    }
};

template <size_t N, class ForwardIt, typename T, typename Compare>
XR_CONSTEXPR_CPP14_OR_INLINE ForwardIt 
lower_bound(ForwardIt first, const T &value, Compare const &compare)
{
    return details::lower_bound_impl<T, Compare>(value, compare)
        .doitfirst(first, std::integral_constant<size_t, N>(),
        std::integral_constant<bool, next_highest_power_of_two(N) - 1 == N>());
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
