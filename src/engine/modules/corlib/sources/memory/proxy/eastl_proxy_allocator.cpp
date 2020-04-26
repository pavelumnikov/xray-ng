// This file is a part of xray-ng engine
//

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/memory.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory, proxy)

//-----------------------------------------------------------------------------------------------------------
/**
*/
void* 
eastl_proxy_allocator::allocate(size_t n, int flags) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(flags);
    XR_DEBUG_ASSERTION(m_allocator != nullptr);

#if defined(_DEBUG)
    XR_DEBUG_ASSERTION(m_allocator_attached != false);
#endif // #if defined(_DEBUG)

    return m_allocator->malloc_impl(n
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("eastl_proxy_allocator")
        XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void* 
eastl_proxy_allocator::allocate(size_t n, size_t alignment, 
    size_t alignment_offset, int flags) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(alignment, alignment_offset, flags);
    XR_DEBUG_ASSERTION(m_allocator != nullptr);

#if defined(_DEBUG)
    XR_DEBUG_ASSERTION(m_allocator_attached != false);
#endif // #if defined(_DEBUG)

    return m_allocator->malloc_impl(n
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("eastl_proxy_allocator")
        XR_DEBUG_PARAMETERS_DEFINITION);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void 
eastl_proxy_allocator::deallocate(void* p, size_t n) const XR_NOEXCEPT
{
    XR_UNREFERENCED_PARAMETER(n);
    XR_DEBUG_ASSERTION(m_allocator != nullptr);

#if defined(_DEBUG)
    XR_DEBUG_ASSERTION(m_allocator_attached != false);
#endif // #if defined(_DEBUG)

    m_allocator->free_impl(p XR_DEBUG_PARAMETERS_DEFINITION);
}

#if defined(_DEBUG)

//-----------------------------------------------------------------------------------------------------------
/**
*/
void 
eastl_proxy_allocator::set_name(utils::string_view s) XR_NOEXCEPT
{
    assert(eastl::size(m_allocator_name) <= s.size());
    eastl::copy(s.cbegin(), s.cend(), m_allocator_name);
}

#endif // #if defined(_DEBUG)

XR_NAMESPACE_END(xr, memory, proxy)
//-----------------------------------------------------------------------------------------------------------
