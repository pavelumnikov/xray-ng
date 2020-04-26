// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/memory/memory_st_arena_allocator.h"
#include "memory_utility_for_arena_win32.h"
#include "corlib/threading/interlocked.h"
#include "corlib/sys/thread.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
*/
st_arena_allocator::st_arena_allocator()
    : m_arena(nullptr)
    , m_user_thread_id(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
st_arena_allocator::~st_arena_allocator()
{
    finalize();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void st_arena_allocator::initialize(size_t size, size_t initial,
    uint32_t const user_thread_id)
{
    XR_DEBUG_ASSERTION_MSG(!m_arena, "arena already initialized");
    XR_DEBUG_ASSERTION_MSG(user_thread_id != 0, "invalid owning thread id");
    m_user_thread_id = user_thread_id;

    SIZE_T const sz_initial = static_cast<SIZE_T>(initial);
    SIZE_T const sz_buffer_size = static_cast<SIZE_T>(size);

    DWORD const dw_initial_flags = HEAP_NO_SERIALIZE | HEAP_GENERATE_EXCEPTIONS;
    m_arena = HeapCreate(dw_initial_flags, sz_initial, sz_buffer_size);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool st_arena_allocator::can_allocate_block(size_t const size) const XR_NOEXCEPT
{
    if(!m_arena) return false;
    // TODO: check if allocating from one block
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t st_arena_allocator::total_size() const XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized");
    HANDLE const heap = static_cast<HANDLE>(m_arena);
    return (mem_usage(heap, nullptr, nullptr));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t st_arena_allocator::allocated_size() const XR_NOEXCEPT
{
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized");
    HANDLE const heap = static_cast<HANDLE>(m_arena);
    return (mem_usage(heap, nullptr, nullptr));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid st_arena_allocator::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_UNREFERENCED_PARAMETER(description);
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before malloc");
    XR_DEBUG_ASSERTION_MSG(m_user_thread_id == sys::current_thread_id(),
        "could not allocate on other thread than specified");

    HANDLE const heap = static_cast<HANDLE>(m_arena);
    pvoid const result = HeapAlloc(heap, 0, size);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid st_arena_allocator::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_UNREFERENCED_PARAMETER(description);
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before reallocation");
    XR_DEBUG_ASSERTION_MSG(m_user_thread_id == sys::current_thread_id(),
        "could not reallocate on other thread than specified");

    HANDLE const heap = static_cast<HANDLE>(m_arena);
    if(!pointer) return HeapAlloc(heap, 0, new_size);
    pvoid const result = HeapReAlloc(heap, 0, pointer, new_size);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void st_arena_allocator::call_free(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before freeing");
    XR_DEBUG_ASSERTION_MSG(m_user_thread_id == sys::current_thread_id(),
        "could not free on other thread than specified");

    HANDLE const heap = static_cast<HANDLE>(m_arena);
    HeapFree(heap, 0, pointer);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void st_arena_allocator::finalize()
{
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before finishing");
    XR_DEBUG_ASSERTION(m_user_thread_id != 0);
    HeapDestroy(m_arena);
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
