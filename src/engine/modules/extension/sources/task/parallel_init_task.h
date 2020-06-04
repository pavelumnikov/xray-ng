// This file is a part of xray-ng engine
//

#pragma once

#include "extension/subsystem.h"
#include "corlib/tasks/task_system.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension, task)

//-----------------------------------------------------------------------------------------------------------
class parallel_init_task
{
public:
    XR_DECLARE_TASK(parallel_init_task,
        tasks::task_stack_request::small_stack,
        tasks::task_priority::default_prority,
        math::color_table::blue);

    parallel_init_task(utils::shared_ptr<subsystem>& system);
    ~parallel_init_task() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(parallel_init_task);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(parallel_init_task);

    void operator()(tasks::execution_context& context);

private:
    utils::shared_ptr<subsystem> m_system;
}; // class sfx_init_task

XR_NAMESPACE_END(xr, extension, task)
//-----------------------------------------------------------------------------------------------------------