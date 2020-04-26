// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
#define XR_EASTL_DEFAULT_PROXY_ALLOCATOR_NAME "eastl_proxy_allocator"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory, proxy)

//-----------------------------------------------------------------------------------------------------------
// eastl_proxy_allocator
//
// Implements an EASTL allocator that uses malloc/free as opposed to 
// new/delete or PPMalloc Malloc/Free. 
//
// Example usage:
//      eastl::vector<int, eastl_proxy_allocator> int_vector;
// 
class eastl_proxy_allocator final
{
public:
    explicit eastl_proxy_allocator(char const* s = XR_EASTL_DEFAULT_PROXY_ALLOCATOR_NAME) XR_NOEXCEPT;

    explicit eastl_proxy_allocator(base_allocator& alloc,
        char const* s = XR_EASTL_DEFAULT_PROXY_ALLOCATOR_NAME) XR_NOEXCEPT;

    eastl_proxy_allocator(const eastl_proxy_allocator& rhs, char const* s) XR_NOEXCEPT;
    XR_DECLARE_DEFAULT_DESTRUCTOR(eastl_proxy_allocator);

    eastl_proxy_allocator(eastl_proxy_allocator const&) XR_NOEXCEPT = default;
    eastl_proxy_allocator(eastl_proxy_allocator&&) XR_NOEXCEPT = default;

    eastl_proxy_allocator& operator=(const eastl_proxy_allocator&) XR_NOEXCEPT = default;
    eastl_proxy_allocator& operator=(eastl_proxy_allocator&&) XR_NOEXCEPT = default;

    bool operator==(const eastl_proxy_allocator&) const XR_NOEXCEPT;
    bool operator!=(const eastl_proxy_allocator&) const XR_NOEXCEPT;

    void* allocate(size_t n, int flags = 0) const XR_NOEXCEPT;
    void* allocate(size_t n, size_t alignment, size_t alignment_offset, int flags = 0) const XR_NOEXCEPT;
    void deallocate(void* p, size_t n) const XR_NOEXCEPT;

    const char* get_name() const XR_NOEXCEPT;
    void set_name(utils::string_view s) XR_NOEXCEPT;
    void set_base_allocator(base_allocator& alloc) XR_NOEXCEPT;

private:
    base_allocator* m_allocator;

#if defined(_DEBUG)
    bool m_allocator_attached;
    char m_allocator_name[64];
#endif // defined(DEBUG)
}; // class eastl_proxy_allocator

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline 
eastl_proxy_allocator::eastl_proxy_allocator(char const* s) XR_NOEXCEPT
    : m_allocator(nullptr)
#if defined(_DEBUG)
    , m_allocator_attached(false)
    , m_allocator_name()
#endif
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline eastl_proxy_allocator::eastl_proxy_allocator(base_allocator& alloc, char const* s) XR_NOEXCEPT
    : m_allocator(&alloc)
#if defined(_DEBUG)
    , m_allocator_attached(true)
    , m_allocator_name()
#endif
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline 
eastl_proxy_allocator::eastl_proxy_allocator(const eastl_proxy_allocator& rhs, char const* s) XR_NOEXCEPT
    : m_allocator(rhs.m_allocator)
#if defined(_DEBUG)
    , m_allocator_attached(true)
    , m_allocator_name()
#endif
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
eastl_proxy_allocator::operator==(const eastl_proxy_allocator&) const XR_NOEXCEPT
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
eastl_proxy_allocator::operator!=(const eastl_proxy_allocator&) const XR_NOEXCEPT
{
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const char*
eastl_proxy_allocator::get_name() const XR_NOEXCEPT
{
#if defined(XR_DEBUG)
    return m_allocator_name;
#else
    return XR_EASTL_DEFAULT_PROXY_ALLOCATOR_NAME;
#endif
}

#if !defined(XR_DEBUG)
//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void 
eastl_proxy_allocator::set_name(utils::string_view s) const XR_NOEXCEPT
{}

#endif // #if !defined(XR_DEBUG)

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void 
eastl_proxy_allocator::set_base_allocator(base_allocator& alloc) XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION(!m_allocator_attached);

    m_allocator = &alloc;
    m_allocator_attached = true;
}

XR_NAMESPACE_END(xr, memory, proxy)
//-----------------------------------------------------------------------------------------------------------
