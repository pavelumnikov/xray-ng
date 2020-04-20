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
namespace xr::memory
{

//-----------------------------------------------------------------------------------------------------------
class base_allocator
{
public:
    virtual ~base_allocator() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(base_allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(base_allocator);

    pvoid malloc_impl(size_t size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION);

    pvoid realloc_impl(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION);

    void free_impl(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION);

    void set_event_listener(base_profiler_event_listener* event_listener);

    virtual bool can_allocate_block(size_t const size) const noexcept = 0;
    virtual size_t total_size() const noexcept = 0;
    virtual size_t allocated_size() const noexcept = 0;

protected:
    constexpr base_allocator() noexcept = default;

    virtual pvoid call_malloc(size_t size_t 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    virtual void call_free(pvoid pointer 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    void on_malloc(pvoid buffer, size_t buffer_size, size_t previous_size, 
        utils::string_view description XR_DEBUG_PARAMETERS_DECLARATION) const;

    void on_free(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) const;

private:
    base_profiler_event_listener* m_event_listener { nullptr };
}; // class base_allocator

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void
base_allocator::set_event_listener(base_profiler_event_listener* event_listener)
{
    m_event_listener = event_listener;
}

} // namespace xr::memory
//-----------------------------------------------------------------------------