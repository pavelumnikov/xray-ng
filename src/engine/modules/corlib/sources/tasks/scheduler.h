// This file is a part of xray-ng engine
//

#pragma once

#include "thread_context.h"
#include "fiber_context.h"
#include "mpmc_queue.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/memory/memory_aligned_allocator.h"
#include "corlib/etl/containers/static_vector.h"
#include "corlib/sys/thread.h"

//------------------------------------------------------------------------------
namespace xr::memory
{
class base_allocator;
} // namespace xr::memory
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::tasks::details
{
struct thread_context;
} // namespace xr::tasks::details
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::tasks
{

//------------------------------------------------------------------------------
// Application can assign task group to task and later wait until group was finished.
class task_group_description
{
public:
    task_group_description() : m_in_progress_task_count { 0 }
    {
#if defined(DEBUG)
        m_debug_is_free = true;
#endif // defined(DEBUG)
    }

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(task_group_description);

    int32_t get_task_count() const
    {
        return threading::atomic_fetch_acq(m_in_progress_task_count);
    }

    int32_t decrement()
    {
        return threading::atomic_dec_fetch_seq(m_in_progress_task_count);
    }

    int32_t increment()
    {
        return threading::atomic_inc_fetch_seq(m_in_progress_task_count);
    }

    int32_t add(int32_t sum)
    {
        return threading::atomic_add_fetch_seq(m_in_progress_task_count, sum);
    }

    volatile int32_t& get_wait_counter()
    {
        return m_in_progress_task_count;
    }

#if defined(DEBUG)
    void set_debug_is_free(bool debug_is_free)
    {
        m_debug_is_free = debug_is_free;
    }

    bool get_debug_is_free() const
    {
        return m_debug_is_free;
    }
#endif // defined(DEBUG)

private:
    volatile int32_t m_in_progress_task_count;

#if defined(DEBUG)
    bool m_debug_is_free;
#endif // defined(DEBUG)
}; // class task_group_description

//------------------------------------------------------------------------------
class task_scheduler : public scheduler
{
public:
    /// \brief Initializes a new instance of the task_scheduler class.
    /// \param workerThreadsCount Worker threads count. Automatically determines the required number of threads if workerThreadsCount set to 0
#ifdef XR_INSTRUMENTED_BUILD
    task_scheduler(memory::base_allocator& alloc, uint32_t workerThreadsCount = 0, base_profiler_event_listener* listener = nullptr);
#else
    task_scheduler(memory::base_allocator& alloc, uint32_t workerThreadsCount = 0);
#endif


    ~task_scheduler();

    void join_worker_threads();

    /// \brief Wait while no more tasks in specific group.
    /// \return true - if no more tasks in specific group. false - if timeout in milliseconds has reached and group still has some tasks.
    signalling_bool wait_group(task_group group, uint32_t milliseconds);

    signalling_bool wait_all(uint32_t milliseconds);

    task_group create_group();
    void release_group(task_group group);

    size_t get_workers_count() const;

    bool is_worker_thread() const;

    memory::base_allocator& get_allocator();

#ifdef XR_INSTRUMENTED_BUILD
    base_profiler_event_listener* get_profiler_event_listener()
    void notify_fibers_created(uint32_t fibers_count);
    void notify_threads_created(uint32_t threads_count);
#endif

protected:
    virtual size_t effective_master_buckets(size_t tasks) override;
    virtual void run_subtasks_on_scheduler(
        etl::containers::array_view<details::task_bucket>& buckets,
        bool restored_from_awaiting) override;

private:
    friend class fiber_context;
    friend struct details::thread_context;

    static constexpr uint32_t max_awaiting_contexts = 4;
    static constexpr uint32_t max_thread_count = 64;
    static constexpr uint32_t max_standard_fibers_count = 256;
    static constexpr uint32_t max_extended_fibers_count = 8;

    struct wait_context_desc
    {
        volatile int32_t* wait_counter { nullptr };
        details::thread_context* thread_ctx { nullptr };
        sys::tick wait_time_ms { 0 };
        uint32_t exit_code { 0 };
    };

    fiber_context* request_fiber_context(details::grouped_task& task);
    void release_fiber_context(fiber_context*&& execution_context);

    void run_tasks_internal(etl::containers::array_view<details::task_bucket>& buckets,
        fiber_context* parent_fiber, bool restored_from_awaiting);

    task_group_description& get_group_desc(task_group group);

    static void worker_thread_main(void* user_data);
    static void scheduler_fiber_main(void* user_data);
    static void scheduler_fiber_wait(void* user_data);
    static bool scheduler_fiber_step(details::thread_context& thread_ctx);
    static void scheduler_fiber_process_task(details::thread_context& context, details::grouped_task& task);
    static void fiber_main(void* user_data);
    static bool try_steal_task(details::thread_context& thread_ctx, details::grouped_task& task);

    static fiber_context* execute_task(details::thread_context& thread_ctx, fiber_context* fiber_ctx);

    //! central memory allocator for tasks system
    memory::aligned_allocator<XR_DEFAULT_MACHINE_ALIGNMENT> m_aligned_allocator;
    //! thread index for new task
    volatile uint32_t m_round_robin_thread_index;
    //! started threads count
    volatile uint32_t m_started_threads_count;
    //! waiting slots for different masters
    eastl::array<sys::thread_id, max_awaiting_contexts> m_awaiting_masters;
    //! waiting slots helper index
    volatile uint32_t m_next_awaiting_master_index;
    //! threads created by task manager
    uint32_t m_threads_count;
    //! all groups task statistic
    task_group_description m_all_groups;
    //! groups pool
    mpmc_queue<task_group, task_group::max_groups_count * 2> m_available_groups;
    //! groups statistics
    task_group_description m_group_stats[task_group::max_groups_count];
    //! standard fibers context
    fiber_context m_standard_fiber_contexts[max_standard_fibers_count];
    //! extended fibers context
    fiber_context m_extended_fiber_contexts[max_extended_fibers_count];
    //! standard fibers pool
    mpmc_queue<fiber_context*, max_standard_fibers_count * 2> m_standard_fibers_available;
    //! extended fibers pool
    mpmc_queue<fiber_context*, max_extended_fibers_count * 2> m_extended_fibers_available;
    //! thread contexts
    details::thread_context* m_thread_context;

#ifdef XR_INSTRUMENTED_BUILD
    base_profiler_event_listener* m_profiler_event_listener;
#endif
}; // class task_scheduler

//------------------------------------------------------------------------------
/**
 */
inline memory::base_allocator&
task_scheduler::get_allocator()
{
    return m_aligned_allocator;
}

#ifdef XR_INSTRUMENTED_BUILD

//------------------------------------------------------------------------------
/**
 */
inline base_profiler_event_listener*
task_scheduler::get_profiler_event_listener()
{
    return m_profiler_event_listener;
}

#endif // XR_INSTRUMENTED_BUILD

} // namespace xr::tasks
//------------------------------------------------------------------------------