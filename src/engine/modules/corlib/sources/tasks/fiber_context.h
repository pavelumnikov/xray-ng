// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/tasks/task_system.h"
#include "corlib/etl/containers/array_view.h"
#include "thread_context.h"
#include "fiber.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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
        etl::array_view<details::task_bucket>& buckets,
        bool restored_from_awaiting) override;

    virtual void run_subtasks_and_yield_impl(
        etl::array_view<details::task_bucket>& buckets);

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
//-----------------------------------------------------------------------------------------------------------
