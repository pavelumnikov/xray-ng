// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/algorithms/constexpr_partition.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::algorithms
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
        auto new_pivot = algorithms::partition(left, right, compare);
        quicksort(left, new_pivot, compare);
        left = new_pivot + 1;
    }
}

} // namespace xr::etl::algorithms
//-----------------------------------------------------------------------------------------------------------
