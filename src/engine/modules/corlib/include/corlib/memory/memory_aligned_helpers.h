// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline pvoid
call_aligned_malloc(memory::base_allocator& alloc, size_t size, size_t align
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    // Allocate necessary memory area client request - size parameter -
    // plus area to store the address of the memory returned by standard
    // malloc().

    size_t aligned_size = size + (align - 1) + sizeof(pvoid);

    // accomodating atleast one aligned memory area by adding align-1
    // size of (void *) is added since we intent to keep the below address
    // in p. This will be retrieved later during free

    pvoid p = alloc.malloc_impl(aligned_size
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);

    if(p != nullptr)
    {
        // Address of the aligned memory according to the align parameter
        pvoid ptr = (pvoid)(((size_t)p + sizeof(pvoid) + align - 1) & ~(align - 1));
        // store the address of the malloc() above the beginning of our total memory area.
        *((pvoid*)((size_t)ptr - sizeof(pvoid))) = p;
        // Return the address of aligned memory
        return ptr;
    }

    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline pvoid
call_aligned_realloc(memory::base_allocator& alloc, pvoid pointer, size_t new_size, size_t align
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_ASSERTION_MSG(false, "Not implemented");
    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void
call_aligned_free(memory::base_allocator& alloc, pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    pvoid ptr = *((pvoid*)((size_t)pointer - sizeof(pvoid)));
    alloc.free_impl(ptr XR_DEBUG_PARAMETERS);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
