// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/algorithms/constexpr_swap.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Iterator, class Compare>
constexpr Iterator
partition(Iterator left, Iterator right, Compare const &compare)
{
    auto pivot = left + (right - left) / 2;
    auto value = *pivot;

    cswap(*right, *pivot);
    for(auto it = left; it < right; ++it)
    {
        if(compare(*it, value))
        {
            cswap(*it, *left);
            ++left;
        }
    }

    cswap(*right, *left);
    return left;
}

} // namespace xr::etl::algorithms
//-----------------------------------------------------------------------------------------------------------
