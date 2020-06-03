// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/allocator_helper.h"
#include "corlib/utils/aligning.h"
#include "corlib/utils/type_traits.h"
#include "corlib/utils/string_view.h"
#include "corlib/memory/memory_debug_parameters.h"
#include "corlib/memory/memory_functions.h"
#include "corlib/memory/profiler_event_listener.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
class base_allocator
{
public:
    virtual ~base_allocator() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(base_allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(base_allocator);

    XR_COMPILER_ALLOCATOR_HINT pvoid malloc_impl(size_t size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION);

    XR_COMPILER_ALLOCATOR_HINT pvoid realloc_impl(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION);

    void free_impl(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION);

    void set_event_listener(base_profiler_event_listener* event_listener);

    virtual bool can_allocate_block(size_t const size) const XR_NOEXCEPT = 0;
    virtual size_t total_size() const XR_NOEXCEPT = 0;
    virtual size_t allocated_size() const XR_NOEXCEPT = 0;

protected:
    XR_CONSTEXPR_CPP14_OR_INLINE base_allocator() XR_NOEXCEPT;

    virtual pvoid call_malloc(size_t size_t 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    virtual void call_free(pvoid pointer 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    void on_malloc(pvoid buffer, size_t buffer_size, size_t previous_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION) const;

    void on_free(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) const;

private:
    base_profiler_event_listener* m_event_listener;
}; // class base_allocator

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE
base_allocator::base_allocator() XR_NOEXCEPT
    : m_event_listener(nullptr)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void
base_allocator::set_event_listener(base_profiler_event_listener* event_listener)
{
    m_event_listener = event_listener;
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------