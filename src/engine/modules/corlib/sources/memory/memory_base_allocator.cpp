// This file is a part of xray-ng engine
//

#include "corlib/memory/memory_allocator_base.h"
#include "EASTL/internal/move_help.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
*/
void
base_allocator::on_malloc(pvoid buffer, size_t buffer_size, size_t previous_size,
    utils::string_view description XR_DEBUG_PARAMETERS_DECLARATION) const
{
    if(base_profiler_event_listener* event_listener = m_event_listener)
    {
        event_listener->on_malloc_done(buffer, buffer_size, previous_size,
            description XR_DEBUG_PARAMETERS);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void
base_allocator::on_free(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) const
{
    if(base_profiler_event_listener* event_listener = m_event_listener)
    {
        event_listener->on_free_done(buffer XR_DEBUG_PARAMETERS);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid base_allocator::malloc_impl(size_t size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    pvoid ptr = call_malloc(size XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    on_malloc(ptr, size, 0 XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    return ptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid base_allocator::realloc_impl(pvoid pointer, size_t new_size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    pvoid ptr = call_realloc(pointer, new_size XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    on_malloc(ptr, new_size, 0 XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    return ptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void base_allocator::free_impl(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    on_free(pointer XR_DEBUG_PARAMETERS);
    call_free(pointer XR_DEBUG_PARAMETERS);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
