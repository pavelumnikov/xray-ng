// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/threading/scoped_lock.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
template<class Allocator, class Mutex>
class memory_synchronized_allocator : public Allocator
{
public:
    using allocator = Allocator;

    memory_synchronized_allocator() XR_NOEXCEPT = default;

protected:
    virtual pvoid call_malloc(size_t size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual void call_free(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION) override;

private:
    threading::mutex m_mutex;
}; // class memory_synchronized_allocator

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class Allocator, class Mutex>
inline pvoid
memory_synchronized_allocator<Allocator, Mutex>::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    threading::scoped_lock lock { m_mutex };
    pvoid out_result = allocator::malloc_impl(size
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);

    return out_result;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class Allocator, class Mutex>
inline pvoid
memory_synchronized_allocator<Allocator, Mutex>::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    threading::scoped_lock lock { m_mutex };
    pvoid out_result = allocator::realloc_impl(pointer, new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);

    return out_result;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class Allocator, class Mutex>
inline void
memory_synchronized_allocator<Allocator, Mutex>::call_free(pvoid pointer
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    threading::scoped_lock lock { m_mutex };
    super::free_impl(pointer XR_DEBUG_PARAMETERS);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
