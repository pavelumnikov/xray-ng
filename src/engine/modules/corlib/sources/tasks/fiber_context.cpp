// This file is a part of xray-ng engine
//

#include "scheduler.h"
#include "fiber.h"
#include "fiber_context.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/utils/static_vector.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

//-----------------------------------------------------------------------------------------------------------
/**
 */
fiber_context::fiber_context()
    : m_thread_context { nullptr }
    , m_task_status { fiber_task_status::UNKNOWN }
    , required_stack { task_stack_request::unknown }
    , children_fibers_count { 0 }
    , parent_fiber { nullptr }
    , fiber_index { 0 }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::set_status(fiber_task_status status)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "Sanity check failed");
    XR_DEBUG_ASSERTION_MSG(m_thread_context->current_thread_id == sys::current_thread_id(),
        "You can change task status only from owner thread");
    m_task_status = status;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
fiber_task_status fiber_context::get_status() const
{
    return m_task_status;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::set_thread_context(details::thread_context* thread_ctx)
{
    if(thread_ctx)
        thread_ctx->last_active_fiber_context = this;

    m_thread_context = thread_ctx;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
details::thread_context* fiber_context::get_thread_context()
{
    return m_thread_context;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::reset()
{
    XR_DEBUG_ASSERTION_MSG(!threading::atomic_fetch_acq(children_fibers_count),
        "Can't release fiber with active children fibers");

    current_task = details::task_desc {};
    parent_fiber = nullptr;
    m_thread_context = nullptr;
    required_stack = task_stack_request::unknown;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::yield()
{
    m_task_status = fiber_task_status::YIELDED;
    fiber& scheduler_fiber = m_thread_context->scheduler_fiber;

#ifdef XR_INSTRUMENTED_BUILD
    m_thread_context->notify_task_execute_state_changed(current_task.debug_color, current_task.debug_id, task_execute_state::suspend, fiber_index);
#endif

    // Yielding, so reset thread context
    m_thread_context = nullptr;

    //switch to scheduler
    fiber::switch_to(system_fiber, scheduler_fiber);

#ifdef XR_INSTRUMENTED_BUILD
    m_thread_context->notify_task_execute_state_changed(current_task.debug_color, current_task.debug_id, task_execute_state::resume, fiber_index);
#endif
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::assert_subtasks_valid(size_t task_count, bool fire_forget)
{
    XR_DEBUG_ASSERTION_MSG(task_count < (details::max_task_buffer_capacity - 1), "Too many tasks per one Run.");
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "thread_context is nullptr");

    if(fire_forget)
    {
        XR_DEBUG_ASSERTION_MSG(m_thread_context->current_scheduler->is_worker_thread(),
            "Can't use run_async outside Task. Use task_scheduler.run_async() instead.");
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t fiber_context::effective_coroutine_buckets(size_t task_count)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "thread_context is nullptr");
    task_scheduler& scheduler = *(m_thread_context->current_scheduler);
    uint32_t input_tasks = static_cast<uint32_t>(task_count);
    size_t bucket_count = eastl::min(scheduler.get_workers_count(), input_tasks);
    return bucket_count;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
pvoid fiber_context::current_effective_buffer()
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "thread_context is nullptr");
    return m_thread_context->desc_buffer;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::run_subtasks_on_scheduler(
    utils::array_view<details::task_bucket>& buckets,
    bool restored_from_awaiting)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "Sanity check failed!");
    task_scheduler& scheduler = *(m_thread_context->current_scheduler);
    scheduler.run_tasks_internal(buckets, nullptr, restored_from_awaiting);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fiber_context::run_subtasks_and_yield_impl(utils::array_view<details::task_bucket>& buckets)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "Sanity check failed!");
    XR_DEBUG_ASSERTION_MSG(m_thread_context->current_scheduler, "Sanity check failed!");
    XR_DEBUG_ASSERTION_MSG(m_thread_context->current_scheduler->is_worker_thread(),
        "Can't use run_subtasks_and_yield outside Task. Use task_scheduler.wait_group() instead.");
    XR_DEBUG_ASSERTION_MSG(m_thread_context->current_thread_id == sys::current_thread_id(),
        "Thread context sanity check failed");

    // add to scheduler
    m_thread_context->current_scheduler->run_tasks_internal(buckets, this, false);

    //
    XR_DEBUG_ASSERTION_MSG(m_thread_context->current_thread_id == sys::current_thread_id(),
        "Thread context sanity check failed");

    // Change status
    m_task_status = fiber_task_status::AWAITING_CHILD;
    fiber& scheduler_fiber = m_thread_context->scheduler_fiber;

#ifdef XR_INSTRUMENTED_BUILD
    m_thread_context->notify_task_execute_state_changed(current_task.debug_color, current_task.debug_id,
        task_execute_state::suspend, (int32)fiber_index);
#endif

    // Yielding, so reset thread context
    m_thread_context = nullptr;

    //switch to scheduler
    fiber::switch_to(system_fiber, scheduler_fiber);

#ifdef XR_INSTRUMENTED_BUILD
    m_thread_context->notify_task_execute_state_changed(current_task.debug_color, current_task.debug_id,
        task_execute_state::resume, (int32)fiber_index);
#endif
}

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------
