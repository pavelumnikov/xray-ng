// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/atomic_backoff.h"
#include "corlib/threading/spin_wait_strategy_traits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

/// <summary>
/// On platforms with proper HW support, this lock may speculatively execute its critical sections, using HW mechanisms
/// to detect real data races and ensure atomicity of the critical sections. In particular, it uses 
/// Intel(R) Transactional Synchronization Extensions(Intel(R) TSX). Without such HW support, it behaves like a 
/// <c>spin_wait_fairness_strategy</c> strategy. It should be used for locking short critical sections where the lock 
/// is contended but the data it protects are not.
/// </summary>
struct spin_wait_speculative_strategy final
    : spin_wait_strategy_traits<uint8_t, false>
{
    void reset(volatile locking_value& locking_value) const XR_NOEXCEPT;
    signalling_bool try_lock(volatile locking_value& locking_value) const XR_NOEXCEPT;
    void lock(volatile locking_value& locking_value) const XR_NOEXCEPT;
    void unlock(volatile locking_value& locking_value) const XR_NOEXCEPT;
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline void 
spin_wait_speculative_strategy::reset(volatile locking_value& locking_value) const XR_NOEXCEPT
{
    locking_value = 0;
}

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------
