// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/etl/function_pointer.h"

namespace xr::memory
{

class crt_allocator final : public base_allocator
{
public:
    crt_allocator();
    bool can_allocate_block(size_t const size) const noexcept override;
    size_t allocated_size() const noexcept override;
    size_t total_size() const noexcept override;

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
    using malloc_ptr_type = etl::function_pointer_t<pvoid, size_t>;
    using free_ptr_type = etl::function_pointer_t<void, pvoid>;
    using realloc_ptr_type = etl::function_pointer_t<pvoid, pvoid, size_t>;

    malloc_ptr_type m_malloc_ptr;
    free_ptr_type m_free_ptr;
    realloc_ptr_type m_realloc_ptr;
}; // class crt_allocator

typedef crt_allocator crt_allocator_type;

} // namespace xr::memory
