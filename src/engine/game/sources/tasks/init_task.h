// This file is a part of xray-ng engine
//

#pragma once

#include "../main/application.h"
#include "extension/context.h"
#include "corlib/tasks/task_system.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, task)

//-----------------------------------------------------------------------------------------------------------
class init_task
{
public:
    XR_DECLARE_TASK(init_task,
        tasks::task_stack_request::small_stack,
        tasks::task_priority::default_prority,
        math::color_table::orange);

    init_task(main::initialize_application_desc const& desc, extension::context& ctx);
    ~init_task() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(init_task);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(init_task);

    void operator()(tasks::execution_context& context);

private:
    main::initialize_application_desc const& m_init_desc;
    extension::context& m_extension_context;
}; // class init_task

XR_NAMESPACE_END(xr, game, task)
//-----------------------------------------------------------------------------------------------------------