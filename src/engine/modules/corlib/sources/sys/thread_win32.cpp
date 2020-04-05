/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

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
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::sys::details
{

WORD constexpr thread_all_processor_groups = 0xffff;

class win32_thread_manager final
{
public:
    win32_thread_manager();

    void set_thread_processor_group(HANDLE thread, uint32_t thread_index) const;
    int32_t find_processor_group_index(int32_t proc_idx) const;
    int32_t get_number_processor_groups() const;
    void move_thread_into_processor_group(HANDLE thread, int32_t group_index) const;
    uint32_t get_processors_count() const;
    uint32_t get_thread_index_for_manager() const;

private:
    // Statically allocate an array for processor group information.
    // Windows 7 supports maximum 4 groups, but let's look ahead a little.
    static constexpr int32_t max_processor_groups = 64;

    //! Initialized internal threading functions
    void initialize_threads_instrumentation();

    static int32_t hole_adjusted(int32_t const hole_idx, int32_t const proc_idx, int32_t const group_idx)
    {
        return (proc_idx + ((hole_idx <= group_idx) ? 1 : 0));
    }

    struct processor_group_info final
    {
        DWORD_PTR mask; //< Affinity mask covering the whole group
        int num_procs; //< Number of processors in the group
        int num_procs_running_total; //< Subtotal of processors in this and preceding groups
    } m_processor_groups[max_processor_groups];

    //! Total number of processor groups in the system
    int32_t m_num_groups;

    //! Index of the group with a slot reserved for the first master thread
    // In the context of multiple processor groups support current implementation
    // defines "the first master thread" as the first thread to invoke
    //
    // TODO: Implement a dynamic scheme remapping workers depending on the pending
    //       master threads affinity.
    int32_t m_hole_index;
};


//------------------------------------------------------------------------------
/**
*/
win32_thread_manager::win32_thread_manager()
    : m_processor_groups {}
    , m_num_groups { 0 }
    , m_hole_index { 0 }
{
    this->initialize_threads_instrumentation();
}

//------------------------------------------------------------------------------
/**
*/
void win32_thread_manager::initialize_threads_instrumentation()
{
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    DWORD_PTR pam, sam, m = 1;
    (void)GetProcessAffinityMask(GetCurrentProcess(), &pam, &sam);

    auto nproc = 0;
    for(size_t idx = 0; idx < sizeof(DWORD_PTR) * CHAR_BIT; idx++, m <<= 1)
    {
        if(pam & m)
        {
            ++nproc;
        }
    }

    auto const number_of_processors = static_cast<int32_t>(si.dwNumberOfProcessors);
    assert(nproc <= number_of_processors);
    // By default setting up a number of processors for one processor group
    m_processor_groups[0].num_procs =
        m_processor_groups[0].num_procs_running_total = nproc;

    // Setting up processor groups in case the process does not restrict affinity 
    // mask and more than one processor group is present

    if(nproc == number_of_processors)
    {
        // The process does not have restricting affinity mask and multiple 
        // processor groups are possible

        this->m_num_groups = static_cast<int32_t>(::GetActiveProcessorGroupCount());
        assert(this->m_num_groups <= max_processor_groups);

        // Fail safety bootstrap. Release versions will limit available concurrency
        // level, while debug ones would assert.
        if(this->m_num_groups > max_processor_groups)
        {
            this->m_num_groups = max_processor_groups;
        }

        if(this->m_num_groups > 1)
        {
            GROUP_AFFINITY ga;
            if(GetThreadGroupAffinity(GetCurrentThread(), &ga))
            {
                this->m_hole_index = static_cast<int32_t>(ga.Group);
            }

            auto nprocs = 0;
            for(auto i = 0; i < this->m_num_groups; ++i)
            {
                auto& pgi = m_processor_groups[i];
                pgi.num_procs = static_cast<int32_t>(::GetActiveProcessorCount(i));
                assert(pgi.num_procs <= static_cast<int>(sizeof(DWORD_PTR)) * CHAR_BIT);

                pgi.mask = pgi.num_procs == sizeof(DWORD_PTR) * CHAR_BIT ?
                    ~static_cast<DWORD_PTR>(0) : (DWORD_PTR(1) << pgi.num_procs) - 1;

                pgi.num_procs_running_total = nprocs += pgi.num_procs;
            }

            assert(nprocs == static_cast<int32_t>(
                ::GetActiveProcessorCount(thread_all_processor_groups)));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void win32_thread_manager::set_thread_processor_group(HANDLE thread, uint32_t thread_index) const
{
    auto const processor_group_index =
        this->find_processor_group_index(thread_index);

    this->move_thread_into_processor_group(thread, processor_group_index);
}

//------------------------------------------------------------------------------
/**
*/
int32_t win32_thread_manager::find_processor_group_index(int32_t proc_idx) const
{
    // In case of over-subscription spread extra workers in a round robin manner
    int32_t hole_idx;
    auto const num_procs =
        m_processor_groups[this->m_num_groups - 1].num_procs_running_total;

    if(proc_idx >= num_procs - 1)
    {
        hole_idx = INT_MAX;
        proc_idx = (proc_idx - num_procs + 1) % num_procs;
    }
    else
    {
        hole_idx = this->m_hole_index;
    }

    // Approximate the likely group index assuming all groups are of the same size
    auto i = proc_idx / m_processor_groups[0].num_procs;

    // Make sure the approximation is a valid group index
    if(i >= this->m_num_groups)
    {
        i = this->m_num_groups - 1;
    }

    // Now adjust the approximation up or down
    if(m_processor_groups[i].num_procs_running_total > hole_adjusted(hole_idx, proc_idx, i))
    {
        while(m_processor_groups[i].num_procs_running_total -
            m_processor_groups[i].num_procs > hole_adjusted(hole_idx, proc_idx, i))
        {
            assert(i > 0);
            --i;
        }
    }
    else
    {
        do
        {
            ++i;
        } while(m_processor_groups[i].num_procs_running_total < hole_adjusted(hole_idx, proc_idx, i));
    }

    assert(i < this->m_num_groups);
    return i;
}

//------------------------------------------------------------------------------
/**
*/
void win32_thread_manager::move_thread_into_processor_group(HANDLE thread, int32_t group_index) const
{
    GROUP_AFFINITY ga =
    {
        m_processor_groups[group_index].mask,
        static_cast<WORD>(group_index),
        { 0, 0, 0 }
    };

    (void)SetThreadGroupAffinity(thread, &ga, nullptr);
}

//------------------------------------------------------------------------------
/**
*/
int32_t win32_thread_manager::get_number_processor_groups() const
{
    return this->m_num_groups;
}

//------------------------------------------------------------------------------
/**
*/
uint32_t win32_thread_manager::get_processors_count() const
{
    return static_cast<uint32_t>(
        m_processor_groups[this->m_num_groups - 1].num_procs_running_total);
}

//------------------------------------------------------------------------------
/**
*/
uint32_t win32_thread_manager::get_thread_index_for_manager() const
{
    GROUP_AFFINITY ga;
    (void)GetThreadGroupAffinity(GetCurrentThread(), &ga);
    return static_cast<uint32_t>(ga.Group);
}

//------------------------------------------------------------------------------
struct thread_data final
{
    using thread_name_type = eastl::wstring_view::value_type;
    static size_t constexpr thread_name_size = 30;

    // This data is set in spawn_thread function

    thread_name_type name_for_debugger[thread_name_size] { 0 };
    memory::base_allocator* allocator { nullptr };
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

static details::win32_thread_manager g_win32_thread_manager;

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

    // Initialize threading functions and system information
    auto const num_processor_groups =
        g_win32_thread_manager.get_number_processor_groups();

    // Allocate thread data
    auto const data = XR_ALLOCATE_OBJECT_T(alloc, details::thread_data, "thread data")();
    XR_DEBUG_ASSERTION(data != nullptr);

    eastl::copy(eastl::begin(debug_thread_name),
        eastl::end(debug_thread_name), eastl::begin(data->name_for_debugger));

    data->allocator = &alloc;
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

#if defined(_DEBUG)
    if(result != 1)
    {
        convert_last_error_to_char(s_spawn_thread_error_buf, sizeof(s_spawn_thread_error_buf));
        assert((result != 0) && s_spawn_thread_error_buf);
    }
#endif

    if(num_processor_groups > 1)
    {
        if(hardware_thread.has_value())
        {
            g_win32_thread_manager.set_thread_processor_group(thread, hardware_thread.value());
        }
    }

    result = ::ResumeThread(thread);

#if defined(_DEBUG)
    if(result != TRUE)
    {
        convert_last_error_to_char(s_spawn_thread_error_buf, sizeof(s_spawn_thread_error_buf));
        assert((result != FALSE) && s_spawn_thread_error_buf);
    }
#endif

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
    return g_win32_thread_manager.get_processors_count();
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
    return g_win32_thread_manager.get_thread_index_for_manager();
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

    uint64_t tm = 0;
    if(timeout.has_value())
        tm = timeout.value();

    auto const result = WaitForMultipleObjectsEx(
        static_cast<uint32_t>(threads_count), threads, do_wait_all, tm, do_alertable);

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