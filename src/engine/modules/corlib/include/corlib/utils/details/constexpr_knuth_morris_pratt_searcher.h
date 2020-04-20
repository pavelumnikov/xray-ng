// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/constexpr_array.h"
#include "corlib/utils/type_traits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::details
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

} // namespace xr::utils::searchers
//-----------------------------------------------------------------------------------------------------------
