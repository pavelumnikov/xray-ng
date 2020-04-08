// This file is a part of xray-ng engine
//

#pragma once

//------------------------------------------------------------------------------
namespace xr::threading
{

template<typename LockingValue, bool HasCacheLineSize>
struct spin_wait_strategy_traits
{
    using locking_value = LockingValue;
    static constexpr auto has_cache_line_size = HasCacheLineSize;
};

} // namespace xr::threading
//------------------------------------------------------------------------------