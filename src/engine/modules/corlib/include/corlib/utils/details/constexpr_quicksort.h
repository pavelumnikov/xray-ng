// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/constexpr_partition.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
/**
 */
template <typename Iterator, class Compare>
XR_CONSTEXPR_CPP14_OR_INLINE void 
quicksort(Iterator left, Iterator right, Compare const &compare)
{
    while(left < right)
    {
        Iterator new_pivot = partition(left, right, compare);
        quicksort(left, new_pivot, compare);
        left = new_pivot + 1;
    }
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
