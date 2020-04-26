// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/sys/thread.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
class st_arena_allocator : public base_allocator
{
public:
    st_arena_allocator();
    virtual ~st_arena_allocator();

    void initialize(size_t size, size_t initial, uint32_t const user_thread_id);

    virtual bool can_allocate_block(size_t const size) const XR_NOEXCEPT;
    virtual size_t total_size() const XR_NOEXCEPT override;
    virtual size_t allocated_size() const XR_NOEXCEPT override;

private:
    virtual pvoid call_malloc(size_t size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual void call_free(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void finalize();

    pvoid m_arena;
    sys::thread_id m_user_thread_id;
}; // class st_arena_allocator

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
