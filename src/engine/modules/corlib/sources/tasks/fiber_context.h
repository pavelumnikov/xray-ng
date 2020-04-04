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

#pragma once

#include "corlib/tasks/task_system.h"
#include "corlib/etl/containers/array_view.h"
#include "thread_context.h"
#include "fiber.h"

//------------------------------------------------------------------------------
namespace xr::tasks
{

//------------------------------------------------------------------------------
// Task can be completed for several reasons.
// For example task was done or someone call yield from the Task body.
enum class fiber_task_status : uint8_t
{
    UNKNOWN = 0,
    RUNNED = 1,
    FINISHED = 2,
    YIELDED = 3,
    AWAITING_CHILD = 4,
};

//------------------------------------------------------------------------------
// Context passed to fiber main function
class fiber_context : public execution_context
{
public:
    fiber_context();
    virtual ~fiber_context() {};

    void set_thread_context(details::thread_context* _threadContext);
    details::thread_context* get_thread_context();

    void set_status(fiber_task_status _taskStatus);
    fiber_task_status get_status() const;
    void reset();

private:

    virtual void yield();
    virtual void assert_subtasks_valid(size_t task_count, bool fire_forget) override;
    virtual size_t effective_coroutine_buckets(size_t task_count) override;
    virtual pvoid current_effective_buffer() override;

    virtual void run_subtasks_on_scheduler(
        etl::containers::array_view<details::task_bucket>& buckets,
        bool restored_from_awaiting) override;

    virtual void run_subtasks_and_yield_impl(
        etl::containers::array_view<details::task_bucket>& buckets);

    // Active thread context (null if fiber context is not executing now)
    details::thread_context* m_thread_context;

    // Active task status
    fiber_task_status m_task_status;

public:
    //! Active task attached to this fiber
    details::task_desc current_task;

    //! Active task group
    task_group current_group;

    //! Requirements for stack
    task_stack_request required_stack;

    //! Number of children fibers
    volatile size_t children_fibers_count;

    //! Parent fiber
    fiber_context* parent_fiber;

    //! System fiber
    fiber system_fiber;

    //! Fiber index in pool
    uint32_t fiber_index;

    //! Prevent false sharing between threads
    uint8_t cacheline[64];
}; // class fiber_context

} // namespace xr::tasks
//------------------------------------------------------------------------------
