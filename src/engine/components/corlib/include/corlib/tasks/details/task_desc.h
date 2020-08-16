// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks)

// forward declarations
class execution_context;

//-----------------------------------------------------------------------------------------------------------
struct task_priority_enum
{
    enum list : uint8_t
    {
        default_prority
    };
}; // struct task_priority_enum
typedef task_priority_enum::list task_priority;

//-----------------------------------------------------------------------------------------------------------
struct task_stack_request_enum
{
    enum list : uint8_t
    {
        unknown,
        small_stack,
        huge_stack
    };
}; // struct task_stack_request_enum
typedef task_stack_request_enum::list task_stack_request;

XR_NAMESPACE_END(xr, tasks)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, tasks, details)

typedef void (*task_entry_function)(execution_context& context, pvoid user_data);

struct task_desc
{
    XR_CONSTEXPR_CPP14_OR_INLINE task_desc() XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE task_desc(task_entry_function exec, pvoid user_data,
        task_stack_request stack_requirements, task_priority priority);

    XR_CONSTEXPR_CPP14_OR_INLINE bool is_valid() const;

    //! task entry point
    task_entry_function task_func;

    //! task user data (task data context)
    pvoid user_data;

    //! stack requirements for task
    task_stack_request required_stack;

    //! priority for task
    task_priority priority;

#ifdef MT_INSTRUMENTED_BUILD
    pstr debug_id { nullptr };
    math::color_table debug_color { math::color_table::blue };
#endif
}; // struct task_desc

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE task_desc::task_desc() XR_NOEXCEPT
    : task_func(nullptr)
    , user_data(nullptr)
    , required_stack(task_stack_request_enum::unknown)
    , priority(task_priority_enum::default_prority)
#ifdef MT_INSTRUMENTED_BUILD
    , debug_id(nullptr)
    , debug_color(Color::Blue)
#endif
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE task_desc::task_desc(task_entry_function func, pvoid ptr,
    task_stack_request stack_req, task_priority priority)
    : task_func(func)
    , user_data(ptr)
    , required_stack(stack_req)
    , priority(priority)
#ifdef MT_INSTRUMENTED_BUILD
    , debug_id(nullptr)
    , debug_color(Color::Blue)
#endif
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool
task_desc::is_valid() const
{
    return (task_func != nullptr);
}

XR_NAMESPACE_END(xr, tasks, details)
//-----------------------------------------------------------------------------------------------------------
