// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/type_traits.h"
#include "EASTL/array.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

// text book implementation from
// https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm

template<typename Type, size_t size> 
class constexpr_knuth_morris_pratt_searcher
{
    XR_STATIC_ASSERT(std::is_string_v<Type>, "Input type must be a string-compatible type");

    eastl::array<ptrdiff_t, size> step_;
    eastl::array<Type, size> needle_;

    static XR_CONSTEXPR_CPP14_OR_INLINE eastl::array<ptrdiff_t, size> build_kmp_cache(Type const (&needle)[size + 1])
    {
        ptrdiff_t cnd = 0;
        eastl::array<ptrdiff_t, size> cache;

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
                    cnd = cache[cnd];

                cnd += 1;
            }
        }
        return cache;
    }

public:
    XR_CONSTEXPR_CPP14_OR_INLINE constexpr_knuth_morris_pratt_searcher(Type const (&needle)[size + 1])
        : step_(build_kmp_cache(needle))
        , needle_(needle)
    {}

    template <class ForwardIterator>
    XR_CONSTEXPR_CPP14_OR_INLINE eastl::pair<ForwardIterator, ForwardIterator> operator()
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
XR_CONSTEXPR_CPP14_OR_INLINE constexpr_knuth_morris_pratt_searcher<Type, N - 1>
make_knuth_morris_pratt_searcher(Type const (&needle)[N])
{
    return constexpr_knuth_morris_pratt_searcher<Type, N - 1>(needle);
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
