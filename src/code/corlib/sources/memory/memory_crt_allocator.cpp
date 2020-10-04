// This file is a part of xray-ng engine
//

#include "corlib/memory/memory_crt_allocator.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid crt_allocator::call_malloc(size_t size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;

#ifdef _DEBUG
    XR_UNREFERENCED_PARAMETER(description);
#endif // #ifdef DEBUG

    XR_DEBUG_ASSERTION(m_malloc_ptr);

    auto const real_size = size;
    pvoid const result = (*m_malloc_ptr)(real_size);

    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid crt_allocator::call_realloc(pvoid pointer, size_t new_size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
#ifdef _DEBUG
    XR_UNREFERENCED_PARAMETER(description);
#endif // #ifdef DEBUG

    if(!new_size)
    {
        free_impl(pointer XR_DEBUG_PARAMETERS);
        return (0);
    }

    XR_DEBUG_ASSERTION(m_realloc_ptr);
    auto const real_size = new_size;
    pvoid const result = (*m_realloc_ptr)(pointer, real_size);

    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void crt_allocator::call_free(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;

    if(!pointer) return;
    XR_DEBUG_ASSERTION(m_free_ptr);
    (*m_free_ptr)(pointer);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
