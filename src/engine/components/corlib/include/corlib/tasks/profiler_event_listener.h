// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

#ifdef XR_INSTRUMENTED_BUILD

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL base_profiler_event_listener
{

public:
    virtual ~base_profiler_event_listener()
    {};

    // Called from main scheduler thread when all fibers has created (notify about fibers count)
    virtual void on_fibers_created(uint32 fibers_count) = 0;

    // Called from main scheduler thread when all threads has created (notify about threads count)
    virtual void on_threads_created(uint32 threads_count) = 0;

    // Called from worker thread context when worker thread created 
    virtual void on_thread_created(uint32 worker_index) = 0;

    // Called from worker thread context when worker thread started
    virtual void on_thread_started(uint32 worker_index) = 0;

    // Called from worker thread context when worker thread stopped
    virtual void on_thread_stoped(uint32 worker_index) = 0;

    // Called from worker thread context when worker thread start to idle
    virtual void on_thread_idle_started(uint32 worker_index) = 0;

    // Called from worker thread context when worker thread return to work from idle
    virtual void on_thread_idle_finished(uint32 worker_index) = 0;

    // Called from thread when thread is waiting for group
    virtual void on_thread_wait_started() = 0;

    // Called from thread when thread is finished waiting for group
    virtual void on_thread_wait_finished() = 0;

    // Called from thread when waiting thread temporary convert to worker
    virtual void on_temporary_worker_thread_join() = 0;

    // Called from thread when temporary worker leave
    virtual void on_temporary_worker_thread_leave() = 0;

    // Called from the worker thread that has change the task execution state
    virtual void on_task_execute_state_changed(math::color_table debug_color, pcstr debug_id, task_execute_state type, uint32 fiber_index) = 0;
};

XR_NAMESPACE_END(xr, tasks)

#endif
