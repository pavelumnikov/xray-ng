// This file is a part of xray-ng engine
//

#pragma once

#include "extension/subsystem.h"
#include "corlib/tasks/task_system.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension, task)

//-----------------------------------------------------------------------------------------------------------
class tick_task
{
public:
    XR_DECLARE_TASK(tick_task,
        tasks::task_stack_request::small_stack,
        tasks::task_priority::default_prority,
        math::color_table::blue_violet);

    tick_task(utils::shared_ptr<subsystem>& system, sys::tick delta);
    ~tick_task() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(tick_task);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(tick_task);

    void operator()(tasks::execution_context& context);

private:
    utils::shared_ptr<subsystem> m_system;
    sys::tick m_delta;
}; // class tick_task

XR_NAMESPACE_END(xr, extension, task)
//-----------------------------------------------------------------------------------------------------------