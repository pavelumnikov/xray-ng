// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/tasks/task_system.h"
#include "../modules/rendering_module.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, task)

//-----------------------------------------------------------------------------------------------------------
class rendering_subsystem_task
{
public:
    XR_DECLARE_TASK(rendering_subsystem_task,
        tasks::task_stack_request::small_stack,
        tasks::task_priority::default_prority,
        math::color_table::orange);

    rendering_subsystem_task(modules::rendering_module& rm);
    ~rendering_subsystem_task() = default;
    
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(rendering_subsystem_task);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(rendering_subsystem_task);

private:
    void operator()(tasks::execution_context& context);
    modules::rendering_module& m_rendering_module;
}; // class rendering_subsystem_task

XR_NAMESPACE_END(xr, game, task)
//-----------------------------------------------------------------------------------------------------------