// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/constexpr_swap.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Iterator, class Compare>
XR_CONSTEXPR_CPP14_OR_INLINE Iterator
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

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
