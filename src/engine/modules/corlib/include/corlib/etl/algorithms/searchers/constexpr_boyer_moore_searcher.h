// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/containers/constexpr_array.h"

//------------------------------------------------------------------------------
namespace  xr::etl::algorithms::searchers
{

//------------------------------------------------------------------------------
template<typename Type, size_t N> 
class constexpr_boyer_moore_searcher
{
public:
    using value_type = Type;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public:
    constexpr constexpr_boyer_moore_searcher(value_type const (&needle)[N + 1]);

    template <class ForwardIterator>
    constexpr ::eastl::pair<ForwardIterator, ForwardIterator> operator()
        (ForwardIterator first, ForwardIterator last) const;

private:
    using skip_table_type = 
        containers::constexpr_array<difference_type, sizeof(value_type) << 8>;
    using suffix_table_type = 
        containers::constexpr_array<difference_type, N>;

    skip_table_type m_skip_table;
    suffix_table_type m_suffix_table;
    containers::constexpr_array<value_type, N> m_needle;

    constexpr auto build_skip_table(value_type const (&needle)[N + 1]);
    constexpr bool is_prefix(value_type const (&needle)[N + 1], size_type pos);
    constexpr size_t suffix_length(value_type const (&needle)[N + 1], size_type pos);
    constexpr auto build_suffix_table(value_type const (&needle)[N + 1]);
};


//------------------------------------------------------------------------------
/**
*/
template<typename Type, size_t N>
constexpr constexpr_boyer_moore_searcher<Type, N - 1>
make_boyer_moore_searcher(Type const (&needle)[N])
{
    return { needle };
}


//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr 
constexpr_boyer_moore_searcher<T, N>::constexpr_boyer_moore_searcher(value_type const (&needle)[N + 1])
    : m_skip_table { build_skip_table(needle) }
    , m_suffix_table { build_suffix_table(needle) }
    , m_needle(needle) 
{}


//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
template <class ForwardIterator>
constexpr ::eastl::pair<ForwardIterator, ForwardIterator> 
constexpr_boyer_moore_searcher<T, N>::operator()(ForwardIterator first, ForwardIterator last) const
{
    if(!N)
    {
        return { first, first + N };
    }

    ForwardIterator iter = (first + N - 1);
    while(iter < last)
    {
        difference_type j = (N - 1);
        while(j > 0 && (*iter == this->m_needle[j]))
        {
            --iter;
            --j;
        }

        if(*iter == this->m_needle[0])
        {
            return { iter, iter + N };
        }

        iter += ::std::max(this->m_skip_table[*iter], this->m_suffix_table[j]);
    }

    return { last, (last + N) };
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr auto 
constexpr_boyer_moore_searcher<T, N>::build_skip_table(value_type const (&needle)[N + 1])
{
    skip_table_type skip_table;

    skip_table.fill(N);
    for(size_type i = 0; i < N - 1; ++i)
    {
        skip_table[needle[i]] -= i + 1;
    }

    return skip_table;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr bool 
constexpr_boyer_moore_searcher<T, N>::is_prefix(value_type const (&needle)[N + 1], size_type pos)
{
    size_type suffixlen = (N - pos);

    for(size_type i = 0; i < suffixlen; i++)
    {
        if(needle[i] != needle[pos + i])
        {
            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**
*/
template<typename T, size_t N>
constexpr constexpr_boyer_moore_searcher<T, N>::size_type
constexpr_boyer_moore_searcher<T, N>::suffix_length(value_type const (&needle)[N + 1], size_type pos)
{
    // increment suffix length slen to the first mismatch or beginning
    // of the word
    for(size_type slen = 0; slen < pos; slen++)
    {
        if(needle[pos - slen] != needle[N - 1 - slen])
        {
            return slen;
        }
    }

    return pos;
}

template<typename T, size_t N>
constexpr auto 
constexpr_boyer_moore_searcher<T, N>::build_suffix_table(value_type const (&needle)[N + 1])
{
    suffix_table_type suffix;
    difference_type last_prefix_index = (N - 1);

    // first loop
    for(difference_type p = (N - 1); p >= 0; p--)
    {
        if(is_prefix(needle, (p + 1)))
        {
            last_prefix_index = p + 1;
        }

        suffix[p] = last_prefix_index + (N - 1 - p);
    }

    // second loop
    for(size_type p = 0; p < (N - 1); p++)
    {
        auto slen = suffix_length(needle, p);
        if(needle[p - slen] != needle[N - 1 - slen])
        {
            suffix[N - 1 - slen] = (N - 1 - p + slen);
        }

    }

    return suffix;
}

} // namespace  xr::etl::algorithms::searchers
//------------------------------------------------------------------------------