// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/signalling_bool.h"
#include "corlib/math/color_table.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/tasks/details/task_desc.h"
#include "corlib/tasks/details/task_group.h"
#include "corlib/tasks/details/work_distribution.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/utils/static_vector.h"
#include "corlib/memory/allocator_macro.h"

// TODO: add profiling scope(or move inside scheduler)
#ifndef XR_SCHEDULER_PROFILER_TASK_SCOPE_CODE_INJECTION
#   define XR_SCHEDULER_PROFILER_TASK_SCOPE_CODE_INJECTION(TYPE, DEBUG_COLOR, SRC_FILE, SRC_LINE)
#endif

#define XR_DECLARE_TASK_IMPL(TYPE, STACK_REQUIREMENTS, TASK_PRIORITY, DEBUG_COLOR) \
    static constexpr xr::tasks::task_stack_request stack_requirements = STACK_REQUIREMENTS; \
    static constexpr xr::tasks::task_priority task_priority = TASK_PRIORITY; \
    static void task_entry_pfn(xr::tasks::execution_context& context, xr::pvoid user_data) \
    { \
        XR_SCHEDULER_PROFILER_TASK_SCOPE_CODE_INJECTION(TYPE, DEBUG_COLOR, __FILE__, __LINE__); \
        TYPE* task = reinterpret_cast<TYPE*>(user_data); \
        task->operator()(context); \
    }

#ifdef XR_INSTRUMENTED_BUILD
#   define XR_DECLARE_TASK(TYPE, STACK_REQUIREMENTS, TASK_PRIORITY, DEBUG_COLOR) \
        static pcstr debug_id = #TYPE; \
        static xr::math::color_table debug_color = DEBUG_COLOR; \
        XR_DECLARE_TASK_IMPL(TYPE, STACK_REQUIREMENTS, TASK_PRIORITY, DEBUG_COLOR)

#else
#   define XR_DECLARE_TASK(TYPE, STACK_REQUIREMENTS, TASK_PRIORITY, DEBUG_COLOR) \
        XR_DECLARE_TASK_IMPL(TYPE, STACK_REQUIREMENTS, TASK_PRIORITY, DEBUG_COLOR)

#endif // XR_INSTRUMENTED_BUILD

//-----------------------------------------------------------------------------------------------------------
namespace xr::memory
{
class base_allocator;
} // namespace xr::memory
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL execution_context
{
public:
    virtual ~execution_context() {};

    template<typename TTask>
    void run_async(task_group group, const TTask* tasks, size_t count);

    template<typename TTask>
    void run_subtasks_and_yield(task_group group, const TTask* tasks, size_t count);

    template<typename TTask, size_t N>
    void run_async(task_group group, TTask(&tasks)[N]);

    template<typename TTask, size_t N>
    void run_subtasks_and_yield(task_group group, TTask(&tasks)[N]);

    template<typename TTask, size_t N>
    void run_async(task_group group, utils::static_vector<TTask, N>& tasks);

    template<typename TTask, size_t N>
    void run_subtasks_and_yield(task_group group, utils::static_vector<TTask, N>& tasks);

    virtual void yield() = 0;

protected:
    virtual void assert_subtasks_valid(size_t task_count, bool fire_forget) = 0;
    virtual size_t effective_coroutine_buckets(size_t task_count) = 0;
    virtual pvoid current_effective_buffer() = 0;

    virtual void run_subtasks_on_scheduler(
        utils::array_view<details::task_bucket>& buckets,
        bool restored_from_awaiting) = 0;

    virtual void run_subtasks_and_yield_impl(
        utils::array_view<details::task_bucket>& buckets) = 0;
}; // class execution_context

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL scheduler
{
public:
    template<typename TTask>
    void run_async(task_group group, const TTask* tasks, size_t count);

    template<typename TTask, size_t N>
    void run_async(task_group group, TTask(&tasks)[N]);

    template<typename TTask, size_t N>
    void run_async(task_group group, utils::static_vector<TTask, N>& tasks);

    virtual task_group create_group() = 0;
    virtual void release_group(task_group group) = 0;

    virtual signalling_bool wait_all(uint32_t milliseconds) = 0;
    virtual signalling_bool wait_group(task_group group, uint32_t milliseconds) = 0;

protected:
    virtual size_t effective_master_buckets(size_t tasks) = 0;
    virtual void run_subtasks_on_scheduler(
        utils::array_view<details::task_bucket>& buckets,
        bool restored_from_awaiting) = 0;
}; // class scheduler

//-----------------------------------------------------------------------------------------------------------
/**
 */
void initialize_tasks(memory::base_allocator& alloc);

//-----------------------------------------------------------------------------------------------------------
/**
 */
void shutdown_tasks();

//-----------------------------------------------------------------------------------------------------------
/**
 */
scheduler& current_scheduler();

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask>
inline void
execution_context::run_async(task_group group, const TTask* tasks, size_t count)
{
    assert_subtasks_valid(count, true);
    pvoid desc_buffer = current_effective_buffer();
    size_t bucket_count = effective_coroutine_buckets(count);

    utils::array_view<details::grouped_task> buffer(desc_buffer, count);
    utils::array_view<details::task_bucket> buckets(
        XR_STACK_ALLOCATE_MEMORY(sizeof(details::task_bucket) * bucket_count), bucket_count);

    details::distibute_descriptions(group, tasks, buffer, buckets);
    run_subtasks_on_scheduler(buckets, false);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask>
inline void
execution_context::run_subtasks_and_yield(task_group group, const TTask* tasks, size_t count)
{
    assert_subtasks_valid(count, false);
    pvoid desc_buffer = current_effective_buffer();
    size_t bucket_count = effective_coroutine_buckets(count);

    utils::array_view<details::grouped_task> buffer(desc_buffer, count);
    utils::array_view<details::task_bucket> buckets(
        XR_STACK_ALLOCATE_MEMORY(sizeof(details::task_bucket) * bucket_count), bucket_count);

    details::distibute_descriptions(group, tasks, buffer, buckets);
    run_subtasks_and_yield_impl(buckets);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask, size_t N>
inline void
execution_context::run_async(task_group group, TTask(&tasks)[N])
{
    run_async(group, tasks, N);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask, size_t N>
inline void
execution_context::run_subtasks_and_yield(task_group group, TTask(&tasks)[N])
{
    run_subtasks_and_yield(group, tasks, N);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask, size_t N>
inline void
execution_context::run_async(task_group group,
    utils::static_vector<TTask, N>& tasks)
{
    run_async(group, tasks.begin(), tasks.size());
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask, size_t N>
inline void
execution_context::run_subtasks_and_yield(task_group group,
    utils::static_vector<TTask, N>& tasks)
{
    run_subtasks_and_yield(group, tasks.begin(), tasks.size());
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask>
inline void
scheduler::run_async(task_group group, const TTask* tasks, size_t count)
{
    size_t bucket_count = effective_master_buckets(count);
    size_t bytes_count_for_grouped_tasks = sizeof(details::grouped_task) * count;
    utils::array_view<details::grouped_task> buffer(
        XR_STACK_ALLOCATE_MEMORY(bytes_count_for_grouped_tasks), count);

    size_t bytes_count_for_task_buckets = sizeof(details::task_bucket) * bucket_count;
    utils::array_view<details::task_bucket> buckets(
        XR_STACK_ALLOCATE_MEMORY(bytes_count_for_task_buckets), bucket_count);

    details::distibute_descriptions(group, tasks, buffer, buckets);
    run_subtasks_on_scheduler(buckets, false);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask, size_t N>
inline void
scheduler::run_async(task_group group, TTask(&tasks)[N])
{
    run_async(group, tasks, N);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename TTask, size_t N>
inline void
scheduler::run_async(task_group group, utils::static_vector<TTask, N>& tasks)
{
    run_async(group, tasks.begin(), tasks.size());
}

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------
