// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/etl/algorithms/constexpr_lowerbound.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

//-----------------------------------------------------------------------------------------------------------
template <size_t N, typename Compare, typename ForwardIt, typename T>
constexpr bool 
binary_search(ForwardIt first, const T &value, Compare const &compare)
{
    ForwardIt where = lower_bound<N>(first, value, compare);
    return (!(where == first + N) && !(compare(value, *where)));
}

} // namespace xr::etl::algorithms
//-----------------------------------------------------------------------------------------------------------
