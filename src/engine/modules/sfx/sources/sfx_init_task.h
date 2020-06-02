// This file is a part of xray-ng engine
//

#pragma once

#include "fmod/fmod_device.h"
#include "corlib/tasks/task_system.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
class sfx_init_task
{
public:
    XR_DECLARE_TASK(sfx_init_task,
        tasks::task_stack_request::small_stack,
        tasks::task_priority::default_prority,
        math::color_table::aqua);

    sfx_init_task(fmod_impl::fmod_device* device);
    ~sfx_init_task() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(sfx_init_task);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(sfx_init_task);

    void operator()(tasks::execution_context& context);

private:
    fmod_impl::fmod_device* m_device;
}; // class sfx_init_task

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------