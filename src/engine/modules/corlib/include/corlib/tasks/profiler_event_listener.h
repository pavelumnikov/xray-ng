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

#include "corlib/types.h"

#ifdef XR_INSTRUMENTED_BUILD

//------------------------------------------------------------------------------
namespace xr::tasks
{

//------------------------------------------------------------------------------
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

} // namespace xr::tasks

#endif
