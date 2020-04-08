// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/spin_wait_strategy_traits.h"
#include "corlib/signalling_bool.h"

//------------------------------------------------------------------------------
namespace xr::threading
{

namespace details
{

struct spin_wait_precise_value_type
{
    volatile uint32_t dest;
    uint32_t exchange;
    uint32_t compare;
};

} // namespace details

/// <summary>
/// </summary>
struct spin_wait_precise_strategy final
    : spin_wait_strategy_traits< details::spin_wait_precise_value_type, false >
{
    void reset(volatile locking_value& locking_value) const noexcept;
    signalling_bool try_lock(volatile locking_value& lockingValue) const noexcept;
    void lock(volatile locking_value& locking_value) const noexcept;
    void unlock(volatile locking_value& locking_value) const noexcept;
};

} // namespace xr::threading
//------------------------------------------------------------------------------