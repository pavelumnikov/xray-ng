// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/utility.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils, details)

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T>
constexpr void
cswap(T &a, T &b)
{
    auto tmp = a;
    a = b;
    b = tmp;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <typename T, typename U>
constexpr void
cswap(eastl::pair<T, U> & a, eastl::pair<T, U> & b)
{
    cswap(a.first, b.first);
    cswap(a.second, b.second);
}

#if XR_COMPILER_SUPPORTS_CPP14
//-----------------------------------------------------------------------------------------------------------
/**
*/
template <class... Tys, size_t... Is>
constexpr void
cswap(eastl::tuple<Tys...> &a, eastl::tuple<Tys...> &b, eastl::index_sequence<Is...>)
{
    using swallow = int[];
    (void)swallow
    {
        (cswap(eastl::get<Is>(a), eastl::get<Is>(b)), 0)...
    };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template <class... Tys>
constexpr void
cswap(eastl::tuple<Tys...> &a, eastl::tuple<Tys...> &b)
{
    cswap(a, b, eastl::make_index_sequence<sizeof...(Tys)>());
}
#endif // #if XR_COMPILER_SUPPORTS_CPP14

XR_NAMESPACE_END(xr, utils, details)
//-----------------------------------------------------------------------------------------------------------
