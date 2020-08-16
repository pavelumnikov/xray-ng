// This file is a part of xray-ng engine
//

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
