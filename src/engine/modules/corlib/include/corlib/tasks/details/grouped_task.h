// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/tasks/details/task_desc.h"
#include "corlib/tasks/details/task_group.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks)

// forward declaration
class fiber_context;

XR_NAMESPACE_END(xr, tasks)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks, details)

//-----------------------------------------------------------------------------------------------------------
struct grouped_task
{
    fiber_context* awaiting_fiber;
    fiber_context* parent_fiber;
    task_desc desc;
    task_group group;

    grouped_task() XR_NOEXCEPT;
    grouped_task(const task_desc& _desc, task_group _group) XR_NOEXCEPT;

    XR_DECLARE_DEFAULT_DESTRUCTOR(grouped_task);
    XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(grouped_task);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(grouped_task);
}; // struct grouped_task

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline grouped_task::grouped_task() XR_NOEXCEPT
    : awaiting_fiber(nullptr)
    , parent_fiber(nullptr)
    , desc()
    , group()
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline grouped_task::grouped_task(const task_desc& _desc, task_group _group) XR_NOEXCEPT
    : awaiting_fiber(nullptr)
    , parent_fiber(nullptr)
    , desc(_desc)
    , group(_group)
{}

XR_NAMESPACE_END(xr, tasks, details)
//-----------------------------------------------------------------------------------------------------------
