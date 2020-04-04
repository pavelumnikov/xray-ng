/*

  Copyright (c) 2018, Pavel Umnikov
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

#include "catch/catch.hpp"
#include "corlib/tasks/task_system.h"
#include "corlib/memory/memory_crt_allocator.h"
#include "corlib/sys/thread.h"
#include "../sources/tasks/scheduler.h"

static xr::memory::crt_allocator main_allocator {};

class not_finished_task_destroy
{
public:
    XR_DECLARE_TASK(not_finished_task_destroy, xr::tasks::task_stack_request::small_stack,
        xr::tasks::task_priority::default_prority, 0);

    void operator()(xr::tasks::execution_context&)
    {
        xr::sys::yield(4);
    }
};

TEST_CASE("CleanupTests: not finished task")
{
    xr::tasks::task_scheduler scheduler { main_allocator };

    not_finished_task_destroy tasks[1024];
    scheduler.run_async(xr::tasks::task_group::get_default_group(), tasks);
    REQUIRE(scheduler.wait_all(3));
}
