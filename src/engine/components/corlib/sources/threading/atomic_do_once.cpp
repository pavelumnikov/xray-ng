// This file is a part of xray-ng engine
//

#include "corlib/threading/atomic_do_once.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading, details)

//-----------------------------------------------------------------------------------------------------------
/**
*/
void run_initializer(void(*f)(), atomic_do_once_state& state)
{
    // Run function handle
    f();
    // Do atomic exchanging of value
    threading::atomic_store_rel(state, do_once_state::executed);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void run_initializer(bool(*f)(), atomic_do_once_state& state)
{
    // Run function handle
    auto const result = f();
    // Do atomic exchanging of value
    result ? threading::atomic_store_rel(state, do_once_state::executed) :
        threading::atomic_store_rel(state, do_once_state::uninitialized);
}

XR_NAMESPACE_END(xr, threading, details)
//-----------------------------------------------------------------------------------------------------------
