// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/memory/memory_crt_allocator.h"
#include "memory_utility_for_arena_win32.h"
#include "corlib/threading/atomic_do_once.h"
#include <new.h>
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
namespace
{

static volatile threading::do_once_state crt_already_initialized 
{ 
    threading::do_once_state::uninitialized
};

//------------------------------------------------------------------------------
/**
*/
int out_of_memory(size_t size)
{
    XR_UNREFERENCED_PARAMETER(size);
    // ("not enough memory for arena [c runtime allocator][request=%d]", size);
    return (1);
}

//------------------------------------------------------------------------------
/**
 */
void initialize_crt_allocator()
{
    _set_new_mode(1);
    _set_new_handler(&out_of_memory);
}

//------------------------------------------------------------------------------
/**
*/
__declspec(noinline) __declspec(allocator) void* 
malloc_proxy_func(size_t size)
{
    HANDLE const handle = ::GetProcessHeap();
    return HeapAlloc(handle, 0, size);
}

//------------------------------------------------------------------------------
/**
*/
__declspec(noinline) __declspec(allocator) void*
realloc_proxy_func(void* p, size_t size)
{
    HANDLE const handle = ::GetProcessHeap();
    if(p == nullptr)
        return malloc_proxy_func(size);

    return HeapReAlloc(handle, 0, p, size);
}

//------------------------------------------------------------------------------
/**
*/
__declspec(noinline) void
free_proxy_func(void* p)
{
    HANDLE const handle = ::GetProcessHeap();
    HeapFree(handle, 0, p);
}

} // namespace `anonymous`
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
*/
crt_allocator::crt_allocator() 
    : m_malloc_ptr { malloc_proxy_func }
    , m_free_ptr { free_proxy_func }
    , m_realloc_ptr { realloc_proxy_func }
{
    XR_DEBUG_ASSERTION(this->m_malloc_ptr != nullptr);
    XR_DEBUG_ASSERTION(this->m_free_ptr != nullptr);
    XR_DEBUG_ASSERTION(this->m_free_ptr != nullptr);

    threading::atomic_do_once(initialize_crt_allocator, crt_already_initialized);
}

//------------------------------------------------------------------------------
/**
*/
bool crt_allocator::can_allocate_block(size_t const size) const noexcept
{
    XR_UNREFERENCED_PARAMETER(size);
    return true;
}

//------------------------------------------------------------------------------
/**
*/
size_t crt_allocator::total_size() const noexcept
{
    HANDLE const heap = ::GetProcessHeap();
    return (mem_usage(heap, nullptr, nullptr));
}

//------------------------------------------------------------------------------
/**
*/
size_t crt_allocator::allocated_size() const noexcept
{
    HANDLE const heap = ::GetProcessHeap();
    return (mem_usage(heap, nullptr, nullptr));
}

} // namespace xr::memory
//------------------------------------------------------------------------------