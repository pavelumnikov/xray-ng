/*

  Copyright (c) 2020, Pavel Umnikov
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

#include "scheduler.h"
#include "fiber.h"
#include "fiber_context.h"
#include "corlib/tasks/details/grouped_task.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/etl/containers/static_vector.h"

//------------------------------------------------------------------------------
namespace xr::tasks
{

//------------------------------------------------------------------------------
/**
 */
fiber_context::fiber_context()
    : m_thread_context { nullptr }
    , m_task_status { fiber_task_status::UNKNOWN }
    , required_stack { task_stack_request::unknown }
    , children_fibers_count { 0 }
    , parent_fiber { nullptr }
{}

//------------------------------------------------------------------------------
/**
 */
void fiber_context::set_status(fiber_task_status status)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "Sanity check failed");
    XR_DEBUG_ASSERTION_MSG(m_thread_context->current_thread_id == sys::current_thread_id(),
        "You can change task status only from owner thread");
    m_task_status = status;
}

//------------------------------------------------------------------------------
/**
 */
fiber_task_status fiber_context::get_status() const
{
    return m_task_status;
}

//------------------------------------------------------------------------------
/**
 */
void fiber_context::set_thread_context(details::thread_context* thread_ctx)
{
    if(thread_ctx)
        thread_ctx->last_active_fiber_context = this;

    m_thread_context = thread_ctx;
}

//------------------------------------------------------------------------------
/**
 */
details::thread_context* fiber_context::get_thread_context()
{
    return m_thread_context;
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/**
 */
size_t fiber_context::effective_coroutine_buckets(size_t task_count)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "thread_context is nullptr");
    task_scheduler& scheduler = *(m_thread_context->current_scheduler);
    size_t bucket_count = eastl::min(scheduler.get_workers_count(), task_count);
    return bucket_count;
}

//------------------------------------------------------------------------------
/**
 */
pvoid fiber_context::current_effective_buffer()
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "thread_context is nullptr");
    return m_thread_context->desc_buffer;
}

//------------------------------------------------------------------------------
/**
 */
void fiber_context::run_subtasks_on_scheduler(
    etl::containers::array_view<details::task_bucket>& buckets,
    bool restored_from_awaiting)
{
    XR_DEBUG_ASSERTION_MSG(m_thread_context, "Sanity check failed!");
    task_scheduler& scheduler = *(m_thread_context->current_scheduler);
    scheduler.run_tasks_internal(buckets, nullptr, restored_from_awaiting);
}

//------------------------------------------------------------------------------
/**
 */
void fiber_context::run_subtasks_and_yield_impl(etl::containers::array_view<details::task_bucket>& buckets)
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
//------------------------------------------------------------------------------
