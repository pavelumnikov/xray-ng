// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/constexpr_boyer_moore_searcher.h"
#include "corlib/utils/details/constexpr_knuth_morris_pratt_searcher.h"
#include "EASTL/algorithm.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type, size_t N>
using search_knuth_morris_pratt = eastl::search<Type, details::constexpr_knuth_morris_pratt_searcher<Type, N>>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type, size_t N>
using search_boyer_moore = eastl::search<Type, details::constexpr_boyer_moore_searcher<Type, N>>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Containers, typename Searcher>
auto search(Containers& c, Searcher& s)
{
    return eastl::search(eastl::begin(c), eastl::end(c), s);
}

} // namespace xr
//-----------------------------------------------------------------------------------------------------------
