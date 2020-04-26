// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/sys/chrono.h"
#include "EASTL/optional.h"

#if defined(XRAY_PLATFORM_WINDOWS)
#   include "corlib/sys/win/min_windows.h"
#endif // defined(XRAY_PLATFORM_WINDOWS)

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys)

//-----------------------------------------------------------------------------------------------------------
enum class thread_priority
{
    low,
    medium, //!< Normal priority, e.g. default priority
    high
}; // enum class thread_priority

#if defined(XRAY_PLATFORM_WINDOWS)
typedef win::HANDLE thread_handle;
typedef uint32_t thread_id;
typedef uint32_t(*thread_function)(pvoid);
XR_CONSTEXPR_CPP14_OR_CONST thread_handle unknown_thread_handle = nullptr;
// See http://blogs.msdn.com/b/oldnewthing/archive/2004/02/23/78395.aspx
XR_CONSTEXPR_CPP14_OR_CONST thread_id invalid_thread_id = 0;

#else
#error "Thread handle is not defined for this platform!"

#endif

//-----------------------------------------------------------------------------------------------------------
/**
*/
thread_handle spawn_thread(thread_function function, void* const arg,
    utils::wstring_view debug_thread_name, thread_priority priority,
    size_t stack_size, eastl::optional<uint32_t> hardware_thread = {});

//-----------------------------------------------------------------------------------------------------------
/**
*/
signalling_bool wait_threads(thread_handle const* threads, size_t threads_count,
    bool wait_all_threads = true, eastl::optional<sys::tick> timeout = {});

//-----------------------------------------------------------------------------------------------------------
/**
*/
void detach_thread(thread_handle thread);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void yield(eastl::optional<sys::tick> timeout = {});

//-----------------------------------------------------------------------------------------------------------
/**
*/
uint32_t core_count();

//-----------------------------------------------------------------------------------------------------------
/**
*/
thread_id current_thread_id();

//-----------------------------------------------------------------------------------------------------------
/**
*/
uint32_t current_thread_affinity();

XR_NAMESPACE_END(xr, sys)
//-----------------------------------------------------------------------------------------------------------
