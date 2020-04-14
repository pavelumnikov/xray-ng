// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/sys/thread.h"
#include "../error_conv_win32.h"
#include <VersionHelpers.h>
#include <process.h>

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

//-----------------------------------------------------------------------------------------------------------
/**
 */
thread_handle spawn_thread(thread_function function, void* const arg,
    eastl::wstring_view debug_thread_name, thread_priority priority,
    size_t stack_size, eastl::optional<uint32_t> hardware_thread)
{
    XR_DEBUG_ASSERTION_MSG(stack_size, "Stack size must be set for thread");

    // Without this flag the 'dwStackSize' parameter to CreateThread specifies 
    // the "Stack Commit Size" and the "Stack Reserve Size" is set to the value 
    // specified at link-time. With this flag the 'dwStackSize' parameter to 
    // CreateThread specifies the "Stack Reserve Size" and the “Stack Commit Size” 
    // is set to the value specified at link-time. For various reasons (some of 
    // which historic) we reserve a large amount of stack space in the project 
    // settings. By setting this flag and by specifying 64 kB for the 
    // "Stack Commit Size" in the project settings we can create new threads 
    // with a much smaller reserved (and committed) stack space. It is very 
    // important that the "Stack Commit Size" is set to a small value in the 
    // project settings. If it is set to a large value we may be both reserving 
    // and committing a lot of memory by setting the 
    // STACK_SIZE_PARAM_IS_A_RESERVATION flag. There are some 50 threads 
    // allocated for normal game play. If, for instance, the commit size is set
    // to 16 MB then by adding this flag we would be reserving and committing 
    // 50 x 16 = 800 MB of memory. On the other hand, if this flag is not set
    // and the "Stack Reserve Size" is set to 16 MB in the project settings, then 
    // we would still be reserving 50 x 16 = 800 MB of virtual address space.
    uint32_t const flags = STACK_SIZE_PARAM_IS_A_RESERVATION | CREATE_SUSPENDED;
    // Create thread and pass thread arguments to function
    auto const thread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr,
        static_cast<uint32_t>(stack_size), function, arg, flags, nullptr));

    XR_DEBUG_ASSERTION(thread != nullptr);
    DWORD result = 0U;

    (void)SetThreadDescription(thread, debug_thread_name.data());
    if(hardware_thread.has_value())
    {
        uint64_t core_id = uint64_t(1) << hardware_thread.value();
        (void)SetThreadAffinityMask(thread, core_id);
    }

    switch(priority)
    {
        case thread_priority::low:
            result = ::SetThreadPriority(thread, THREAD_PRIORITY_LOWEST);
            break;

        case thread_priority::medium:
            result = ::SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);
            break;

        case thread_priority::high:
            result = ::SetThreadPriority(thread, THREAD_PRIORITY_HIGHEST);
            break;
    };
    result = ::ResumeThread(thread);
    return thread;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t core_count()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int)si.dwNumberOfProcessors;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
thread_id current_thread_id()
{
    return static_cast<thread_id>(::GetCurrentThreadId());
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t current_thread_affinity()
{
    GROUP_AFFINITY ga;
    (void)GetThreadGroupAffinity(GetCurrentThread(), &ga);
    return static_cast<uint32_t>(ga.Group);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void yield(eastl::optional<sys::tick> timeout)
{
    auto constexpr alertable = TRUE;

    if(!timeout.has_value())
    {
#if defined(_DEBUG)
        auto const result = SwitchToThread();
        XR_UNREFERENCED_PARAMETER(result);
#else
        ::SwitchToThread();
#endif
    }
    else
    {
        uint64_t t = timeout.value();
        ::SleepEx(static_cast<uint32_t>(t), alertable);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
signalling_bool wait_threads(thread_handle const* threads,
    size_t const threads_count, bool const wait_all_threads,
    eastl::optional<sys::tick> timeout)
{
    // this is maximum count of objects for WaitForMultipleObjectsEx
    DWORD constexpr default_handles_count = 64;
    XR_DEBUG_ASSERTION(threads_count < default_handles_count);

    auto const do_wait_all = wait_all_threads ? TRUE : FALSE;
    auto const constexpr do_alertable = TRUE; // all threads must be alertable

    uint64_t tm = INFINITE;
    if(timeout.has_value())
        tm = timeout.value();

    auto const result = WaitForMultipleObjectsEx(
        static_cast<uint32_t>(threads_count), threads, do_wait_all,
        static_cast<uint32_t>(tm), do_alertable);

    if(result == WAIT_OBJECT_0)
        return true;

    if((result < (WAIT_OBJECT_0 + threads_count)) && !wait_all_threads)
        return true;

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void detach_thread(thread_handle thread)
{
    XR_DEBUG_ASSERTION(thread != unknown_thread_handle);
    // TODO: check for resulting output
    (void)CloseHandle(thread);
}

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------
