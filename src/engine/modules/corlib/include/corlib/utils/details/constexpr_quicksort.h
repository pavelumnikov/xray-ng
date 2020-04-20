// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/constexpr_partition.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils::details
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename Iterator, class Compare>
constexpr void 
quicksort(Iterator left, Iterator right, Compare const &compare)
{
    while(left < right)
    {
        auto new_pivot = partition(left, right, compare);
        quicksort(left, new_pivot, compare);
        left = new_pivot + 1;
    }
}

} // namespace xr::utils::details
//-----------------------------------------------------------------------------------------------------------
