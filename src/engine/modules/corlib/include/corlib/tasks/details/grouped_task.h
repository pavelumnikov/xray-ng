// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/tasks/details/task_desc.h"
#include "corlib/tasks/details/task_group.h"

//------------------------------------------------------------------------------
namespace xr::tasks
{
class fiber_context;
} // namespace xr::tasks
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::tasks::details
{

//------------------------------------------------------------------------------
struct grouped_task
{
    fiber_context* awaiting_fiber { nullptr };
    fiber_context* parent_fiber { nullptr };
    task_desc desc {};
    task_group group {};

    grouped_task() = default;
    grouped_task(const task_desc& _desc, task_group _group);

    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(grouped_task);
    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(grouped_task);
}; // struct grouped_task

//------------------------------------------------------------------------------
/**
 */
inline grouped_task::grouped_task(const task_desc& _desc, task_group _group)
    : awaiting_fiber { nullptr }
    , parent_fiber { nullptr }
    , desc { _desc }
    , group { _group }
{}

} // namespace xr::tasks::details
//------------------------------------------------------------------------------
