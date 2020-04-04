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

#include "corlib/tasks/task_system.h"
#include "corlib/sys/thread.h"
#include "corlib/memory/uninitialized_reference.h"
#include "scheduler.h"

//------------------------------------------------------------------------------
namespace xr::tasks
{

//------------------------------------------------------------------------------
static memory::uninitialized_reference<task_scheduler> main_scheduler;

#define XR_TASKS_CHECK_SCHEDULER_INITIALIZED \
    XR_DEBUG_ASSERTION_MSG(main_scheduler.is_constructed(), "Task scheduler is not initialized")

//------------------------------------------------------------------------------
/**
 */
void initialize_tasks(memory::base_allocator& alloc)
{
    XR_DEBUG_ASSERTION_MSG(!main_scheduler.is_constructed(),
        "Task scheduler already initialized");
    memory::construct_reference(main_scheduler, alloc, sys::core_count());
}

//------------------------------------------------------------------------------
/**
 */
void shutdown_tasks()
{
    XR_TASKS_CHECK_SCHEDULER_INITIALIZED;
    memory::destruct_reference(main_scheduler);
}

//------------------------------------------------------------------------------
/**
 */
scheduler& current_scheduler()
{
    XR_TASKS_CHECK_SCHEDULER_INITIALIZED;
    return main_scheduler.ref();
}

} // namespace xr::tasks
//------------------------------------------------------------------------------