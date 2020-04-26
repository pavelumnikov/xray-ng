// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/namespaces.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

template<typename LockingValue, bool HasCacheLineSize>
struct spin_wait_strategy_traits
{
    using locking_value = LockingValue;
    static constexpr auto has_cache_line_size = HasCacheLineSize;
};

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
