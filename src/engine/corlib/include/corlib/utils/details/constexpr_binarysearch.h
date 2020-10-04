// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/constexpr_lowerbound.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
template <size_t N, typename Compare, typename ForwardIt, typename T>
XR_CONSTEXPR_CPP14_OR_INLINE bool 
binary_search(ForwardIt first, const T &value, Compare const &compare)
{
    ForwardIt where = lower_bound<N>(first, value, compare);
    return (!(where == first + N) && !(compare(value, *where)));
}

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
