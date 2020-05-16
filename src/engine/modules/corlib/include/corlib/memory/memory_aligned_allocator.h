// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_aligned_helpers.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
template<size_t kAlignment>
class aligned_allocator final : public base_allocator
{
    using super = base_allocator;

public:
    aligned_allocator(memory::base_allocator& alloc);
    XR_CONSTEXPR_CPP14_OR_INLINE size_t alignment() const;

    virtual bool can_allocate_block(size_t const size) const XR_NOEXCEPT override;
    virtual size_t allocated_size() const XR_NOEXCEPT override;
    virtual size_t total_size() const XR_NOEXCEPT override;

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
    memory::base_allocator& m_allocator;
}; // class crt_allocator

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline aligned_allocator<kAlignment>::aligned_allocator(memory::base_allocator& alloc)
    : m_allocator { alloc }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline bool
aligned_allocator<kAlignment>::can_allocate_block(size_t const size) const XR_NOEXCEPT
{
    return m_allocator.can_allocate_block(size + kAlignment);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline size_t
aligned_allocator<kAlignment>::allocated_size() const XR_NOEXCEPT
{
    return m_allocator.allocated_size();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline size_t
aligned_allocator<kAlignment>::total_size() const XR_NOEXCEPT
{
    return m_allocator.total_size();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
XR_CONSTEXPR_CPP14_OR_INLINE size_t 
aligned_allocator<kAlignment>::alignment() const
{
    return kAlignment;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline pvoid
aligned_allocator<kAlignment>::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    return call_aligned_malloc(m_allocator, size, kAlignment
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline pvoid
aligned_allocator<kAlignment>::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    return call_aligned_realloc(m_allocator, pointer, new_size, kAlignment
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline void aligned_allocator<kAlignment>::call_free(pvoid pointer
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    call_aligned_free(m_allocator, pointer XR_DEBUG_PARAMETERS_DEFINITION);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
