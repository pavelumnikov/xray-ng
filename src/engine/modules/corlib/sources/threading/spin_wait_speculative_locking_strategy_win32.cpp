// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/threading/spin_wait_speculative_locking_strategy.h"
#include "corlib/threading/atomic_backoff_helpers.h"
#include "../os_include_win32.h"

extern "C" int8_t _TSXTryLockTransactive( volatile void* ptr );
extern "C" void _TSXUnlockTransactive( volatile void* ptr );

//------------------------------------------------------------------------------
namespace xr::threading
{

//------------------------------------------------------------------------------
/**
*/
void 
spin_wait_speculative_strategy::lock(volatile locking_value& locking_value) const noexcept
{
    for(;;)
    {
        backoff_while_equals< default_atomic_backoff >(locking_value, 1);

        if(::_TSXTryLockTransactive(&locking_value))
            return;

        // Another thread acquired the lock "for real".
        // To avoid the "lemming" effect, we abort the transaction.
        ::_mm_pause();
    }
}

//------------------------------------------------------------------------------
/**
*/
signalling_bool
spin_wait_speculative_strategy::try_lock(volatile locking_value& locking_value) const noexcept
{
    auto const result = ::_TSXTryLockTransactive(&locking_value) != 0;

    // to avoid the "lemming" effect, we need to abort the transaction
    // if  _TSXTryLockTransactive returns false (i.e., someone else
    // has acquired the mutex non-speculatively).
    if(!result)
        _mm_pause();

    return result;
}

//------------------------------------------------------------------------------
/**
*/
void
spin_wait_speculative_strategy::unlock(volatile locking_value& locking_value) const noexcept
{
    ::_TSXUnlockTransactive(&locking_value);
}

} // namespace xr::threading
//------------------------------------------------------------------------------