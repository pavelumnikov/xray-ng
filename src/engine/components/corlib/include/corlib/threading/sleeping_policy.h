// This file is a part of xray-ng engine
//

#pragma once 

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

//-----------------------------------------------------------------------------------------------------------
struct sleep_thread_switch final
{};

//-----------------------------------------------------------------------------------------------------------
struct sleep_spin_wait final
{};

//-----------------------------------------------------------------------------------------------------------
// Sleeping policy forward declaration
template< typename T > struct sleep_policy;

//-----------------------------------------------------------------------------------------------------------
template<>
struct sleep_policy<sleep_thread_switch> final
{
    void operator()(size_t delay)
    {
        XR_DEBUG_ASSERTION(delay == 0);
        __XR_SLEEPING_SWITCH_TO_THREAD;
    }
};

//-----------------------------------------------------------------------------------------------------------
template<>
struct sleep_policy<sleep_spin_wait> final
{
    void operator()(size_t delay)
    {
        XR_DEBUG_ASSERTION(delay > 0);
        while(delay--)
            __XR_SLEEPING_PAUSE;
    }
};

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------