// This file is a part of xray-ng engine
//

#if !defined(XRAY_PLATFORM_WINDOWS)
#   error "This code is supported by Windows platform!"
#endif // !defined(XRAY_PLATFORM_WINDOWS)

#include "corlib/sys/thread.h"
#include "corlib/etl/type_conversions.h"
#include "EASTL/internal/copy_help.h"
#include "corlib/sys/win/min_windows.h"
#include "corlib/memory/allocator_macro.h"
#include "../error_conv_win32.h"
#include <VersionHelpers.h>
#include <process.h>

//------------------------------------------------------------------------------
namespace xr::sys::details
{

//------------------------------------------------------------------------------
struct thread_data final
{
    using thread_name_type = eastl::wstring_view::value_type;
    static size_t constexpr thread_name_size = 30;

    // This data is set in spawn_thread function

    thread_name_type name_for_debugger[thread_name_size] { 0 };
    memory::base_allocator* allocator { nullptr };
    eastl::optional<uint32_t> hardware_index {};
    thread_function function { nullptr };
    void* argument { nullptr };

    // This data is set internally in thread on first access

    // thread identity index
    DWORD thread_index { 0 };
}; // struct thread_arguments

} // namespace xr::sys::details
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
namespace
{

// initializer state for thread-local data
INIT_ONCE g_thread_local_init = INIT_ONCE_STATIC_INIT; // Static initialization
// thread-local data for thread
DWORD g_thread_local_data { 0 };

BOOL CALLBACK initialize_thread_local_data(PINIT_ONCE init_once,
    PVOID parameter, PVOID *context)
{
    XR_DEBUG_ASSERTION(g_thread_local_data == 0);
    g_thread_local_data = TlsAlloc();
    auto const created = g_thread_local_data != TLS_OUT_OF_INDEXES;

    if(!created)
        return 0;

    return 1;
}

//------------------------------------------------------------------------------
/**
 */
unsigned __stdcall thread_procedure_proxy(void* const argument)
{
    auto* data = reinterpret_cast<details::thread_data*>(argument);
    XR_DEBUG_ASSERTION(data != nullptr);

    memory::base_allocator* allocator = data->allocator;
    XR_DEBUG_ASSERTION(allocator != nullptr);

    if(data->hardware_index.has_value())
    {
        uint64_t hardware_code_id = uint64_t(1) << data->hardware_index.value();
        SetThreadAffinityMask(GetCurrentThread(), hardware_code_id);
    }

    XR_DEBUG_ASSERTION(data->thread_index == 0);
    data->thread_index = GetCurrentThreadId();
    XR_DEBUG_ASSERTION(data->thread_index != 0);

    (void)SetThreadDescription(GetCurrentThread(), data->name_for_debugger);

    data->function(data->argument);
    XR_DEALLOCATE_MEMORY(*allocator, data);

    return 0;
}

} // namespace anonymous
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 */
thread_handle spawn_thread(memory::base_allocator& alloc,
    thread_function function, void* const arg,
    eastl::wstring_view debug_thread_name, thread_priority priority,
    size_t stack_size, eastl::optional<uint32_t> hardware_thread)
{
    XR_DEBUG_ASSERTION_MSG(stack_size, "Stack size must be set for thread");
    XR_UNREFERENCED_PARAMETER(hardware_thread);

#if defined(_DEBUG)
    size_t constexpr s_spawn_thread_error_len = 1024;
    static thread_local char s_spawn_thread_error_buf[s_spawn_thread_error_len];
#endif

    // Initialize thread local data
    auto const execute_once_result = InitOnceExecuteOnce(
        &g_thread_local_init, initialize_thread_local_data,
        nullptr, nullptr);

    XR_DEBUG_ASSERTION_MSG(execute_once_result, "Could not initialize TLS data!");

    // Allocate thread data
    auto const data = XR_ALLOCATE_OBJECT_T(alloc, details::thread_data, "thread data")();
    XR_DEBUG_ASSERTION(data != nullptr);

    eastl::copy(eastl::begin(debug_thread_name),
        eastl::end(debug_thread_name), eastl::begin(data->name_for_debugger));

    data->allocator = &alloc;
    data->hardware_index = hardware_thread;
    data->function = function;
    data->argument = arg;

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
        static_cast<uint32_t>(stack_size), thread_procedure_proxy, data, flags, nullptr));

    XR_DEBUG_ASSERTION(thread != nullptr);
    auto result = 0U;

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

#if !defined(_DEBUG)
    XR_UNREFERENCED_PARAMETER(result);
#endif

    return thread;
}

//------------------------------------------------------------------------------
/**
 */
uint32_t core_count()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int)si.dwNumberOfProcessors;
}

//------------------------------------------------------------------------------
/**
 */
thread_id current_thread_id()
{
    auto const r = ::GetCurrentThreadId();
    return etl::implicit_cast<thread_id>(r);
}

//------------------------------------------------------------------------------
/**
 */
uint32_t current_thread_affinity()
{
    GROUP_AFFINITY ga;
    (void)GetThreadGroupAffinity(GetCurrentThread(), &ga);
    return static_cast<uint32_t>(ga.Group);
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/**
 */
void detach_thread(thread_handle thread)
{
    XR_DEBUG_ASSERTION(thread != unknown_thread_handle);
    // TODO: check for resulting output
    CloseHandle(thread);
}

//------------------------------------------------------------------------------
/**
 */
eastl::wstring_view get_current_thread_name()
{
    auto const tls_slot = g_thread_local_data;
    XR_DEBUG_ASSERTION(tls_slot != TLS_OUT_OF_INDEXES);

    auto const raw = TlsGetValue(tls_slot);
    XR_DEBUG_ASSERTION(raw != nullptr);

    auto data = static_cast<details::thread_data*>(raw);
    return data->name_for_debugger;
}

} // namespace xr::threading
//------------------------------------------------------------------------------