// This file is a part of xray-ng engine
//

#include "corlib/memory/memory_crt_allocator.h"
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
/**
*/
pvoid crt_allocator::call_malloc(size_t size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;

#ifdef _DEBUG
    XR_UNREFERENCED_PARAMETER(description);
#endif // #ifdef DEBUG

    assert(this->m_malloc_ptr);

    auto const real_size = size;
    pvoid const result = (*this->m_malloc_ptr)(real_size);

    return result;
}

//------------------------------------------------------------------------------
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
        this->free_impl(pointer XR_DEBUG_PARAMETERS);
        return (0);
    }

    assert(this->m_realloc_ptr);
    auto const real_size = new_size;
    pvoid const result = (*this->m_realloc_ptr)(pointer, real_size);

    return result;
}

//------------------------------------------------------------------------------
/**
*/
void crt_allocator::call_free(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;

    if(!pointer)
        return;

    assert(this->m_free_ptr);
    (*this->m_free_ptr)(pointer);
}

} // namespace xr::memory
//------------------------------------------------------------------------------