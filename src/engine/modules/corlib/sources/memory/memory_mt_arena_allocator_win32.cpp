// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/memory/memory_mt_arena_allocator.h"
#include "memory_utility_for_arena_win32.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::memory
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
mt_arena_allocator::~mt_arena_allocator()
{
    finalize();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void mt_arena_allocator::initialize(size_t size, size_t initial)
{
    XR_DEBUG_ASSERTION_MSG(!m_arena, "arena already initialized");

    SIZE_T const sz_initial = static_cast<SIZE_T>(initial);
    SIZE_T const sz_buffer_size = static_cast<SIZE_T>(size);

    DWORD const dw_initial_flags = 0;
    m_arena = HeapCreate(dw_initial_flags, sz_initial, sz_buffer_size);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool mt_arena_allocator::can_allocate_block(size_t const size) const noexcept
{
    if(!m_arena) return false;
    // TODO: check if allocating from one block
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t mt_arena_allocator::total_size() const noexcept
{
    HANDLE const heap = static_cast<HANDLE>(m_arena);
    return (mem_usage(heap, nullptr, nullptr));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t mt_arena_allocator::allocated_size() const noexcept
{
    HANDLE const heap = static_cast<HANDLE>(m_arena);
    return (mem_usage(heap, nullptr, nullptr));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid mt_arena_allocator::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
    XR_UNREFERENCED_PARAMETER(description);
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before malloc");

    HANDLE const heap = static_cast<HANDLE>(m_arena);
    pvoid const result = HeapAlloc(heap, 0, size);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid mt_arena_allocator::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_UNREFERENCED_PARAMETER(description);
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before reallocation");

    HANDLE const heap = static_cast<HANDLE>(m_arena);
    if(!pointer) return HeapAlloc(heap, 0, new_size);
    pvoid const result = HeapReAlloc(heap, 0, pointer, new_size);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void mt_arena_allocator::call_free(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before freeing");

    HANDLE const heap = static_cast<HANDLE>(m_arena);
    HeapFree(heap, 0, pointer);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void mt_arena_allocator::finalize()
{
    XR_DEBUG_ASSERTION_MSG(m_arena, "arena must be initialized before finishing");
    HeapDestroy(m_arena);
}

} // namespace xr::memory
//-----------------------------------------------------------------------------------------------------------
