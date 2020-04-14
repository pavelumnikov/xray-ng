// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks::details
{

// forward declarations
struct grouped_task;

//-----------------------------------------------------------------------------------------------------------
struct task_bucket
{
    grouped_task* tasks { nullptr };
    size_t count { 0 };

    task_bucket() = default;
    task_bucket(grouped_task* _tasks, size_t _count);

    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(task_bucket);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(task_bucket);
}; // struct task_bucket

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_bucket::task_bucket(grouped_task* t, size_t count)
    : tasks { t }
    , count { count }
{}

} // namespace xr::tasks::details
//-----------------------------------------------------------------------------------------------------------
