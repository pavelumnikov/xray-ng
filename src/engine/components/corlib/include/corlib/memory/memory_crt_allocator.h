// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
class crt_allocator final : public base_allocator
{
public:
    crt_allocator();
    bool can_allocate_block(size_t const size) const XR_NOEXCEPT override;
    size_t allocated_size() const XR_NOEXCEPT override;
    size_t total_size() const XR_NOEXCEPT override;

protected:
    pvoid call_malloc(size_t size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    pvoid call_realloc(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void call_free(pvoid pointer 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

private:
    typedef pvoid(*malloc_ptr_type)(size_t);
    typedef void(*free_ptr_type)(pvoid);
    typedef pvoid(*realloc_ptr_type)(pvoid, size_t);

    malloc_ptr_type m_malloc_ptr;
    free_ptr_type m_free_ptr;
    realloc_ptr_type m_realloc_ptr;
}; // class crt_allocator

//-----------------------------------------------------------------------------------------------------------
typedef crt_allocator crt_allocator_type;

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
