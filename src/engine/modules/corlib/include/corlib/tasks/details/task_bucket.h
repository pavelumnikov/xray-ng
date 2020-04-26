// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks, details)

// forward declarations
struct grouped_task;

//-----------------------------------------------------------------------------------------------------------
struct task_bucket
{
    grouped_task* tasks;
    size_t count;

    XR_CONSTEXPR_CPP14_OR_INLINE task_bucket() XR_NOEXCEPT;
    task_bucket(grouped_task* _tasks, size_t _count) XR_NOEXCEPT;

    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(task_bucket);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(task_bucket);
}; // struct task_bucket

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE task_bucket::task_bucket() XR_NOEXCEPT
    : tasks(nullptr)
    , count(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline task_bucket::task_bucket(grouped_task* t, size_t count) XR_NOEXCEPT
    : tasks(t)
    , count(count)
{}

XR_NAMESPACE_END(xr, tasks, details)
//-----------------------------------------------------------------------------------------------------------
