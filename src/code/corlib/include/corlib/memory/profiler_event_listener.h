// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/string_view.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL base_profiler_event_listener
{
public:
    virtual ~base_profiler_event_listener()
    {};

    // Called from memory allocator on allocation/reallocation
    virtual void on_malloc_done(pvoid buffer, size_t buffer_size, size_t previous_size,
        utils::string_view description XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    // Called from memory allocator on free
    virtual void on_free_done(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) = 0;
}; // class base_profiler_event_listener

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------