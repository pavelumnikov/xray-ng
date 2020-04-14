// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"

namespace xr::memory
{

class mt_arena_allocator final : public base_allocator 
{
public:
    mt_arena_allocator() = default;
    virtual ~mt_arena_allocator();

    void initialize(size_t size, size_t initial);

    virtual bool can_allocate_block(size_t const size) const noexcept override;
    virtual size_t total_size() const noexcept override;
    virtual size_t allocated_size() const noexcept override;

private:
    pvoid call_malloc(size_t size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    pvoid call_realloc(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void call_free(pvoid pointer 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void finalize();

    pvoid m_arena { nullptr };
}; // class mt_arena_allocator

} // namespace xr::memory
