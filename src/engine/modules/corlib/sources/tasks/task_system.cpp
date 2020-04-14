// This file is a part of xray-ng engine
//

#include "corlib/tasks/task_system.h"
#include "corlib/sys/thread.h"
#include "corlib/memory/uninitialized_reference.h"
#include "scheduler.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

//-----------------------------------------------------------------------------------------------------------
static memory::uninitialized_reference<task_scheduler> main_scheduler;

#define XR_TASKS_CHECK_SCHEDULER_INITIALIZED \
    XR_DEBUG_ASSERTION_MSG(main_scheduler.is_constructed(), "Task scheduler is not initialized")

//-----------------------------------------------------------------------------------------------------------
/**
 */
void initialize_tasks(memory::base_allocator& alloc)
{
    XR_DEBUG_ASSERTION_MSG(!main_scheduler.is_constructed(),
        "Task scheduler already initialized");
    memory::construct_reference(main_scheduler, alloc, sys::core_count());
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void shutdown_tasks()
{
    XR_TASKS_CHECK_SCHEDULER_INITIALIZED;
    memory::destruct_reference(main_scheduler);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
scheduler& current_scheduler()
{
    XR_TASKS_CHECK_SCHEDULER_INITIALIZED;
    return main_scheduler.ref();
}

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------
