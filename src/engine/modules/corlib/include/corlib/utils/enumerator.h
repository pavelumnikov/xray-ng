// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/utility.h"
#include "EASTL/tuple.h"

namespace xr::utils
{

template<typename T, typename TIter = decltype(eastl::begin(eastl::declval<T>())),
    typename = decltype(eastl::end(eastl::declval<T>()))>
auto enumerate(T&& iterable)
{
    struct iterator
    {
        size_t i;
        TIter iter;

        iterator(size_t const i, TIter iter) : i(i), iter(iter)
        {};

        bool operator != (const iterator& rhs) const
        {
            return iter != rhs.iter;
        }

        void operator ++ ()
        {
            ++i; ++iter;
        }
        auto operator * () const
        {
            return eastl::tie(i, *iter);
        }
    };

    struct iterable_wrapper
    {
        T iterable;
        auto begin()
        {
            return iterator { 0, eastl::begin(iterable) };
        }
        auto end()
        {
            return iterator { 0, eastl::end(iterable) };
        }
    };

    return iterable_wrapper { eastl::forward<T>(iterable) };
}

} // namespace xr
