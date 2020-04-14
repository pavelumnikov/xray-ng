// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks
{

// forward declarations
class execution_context;

//-----------------------------------------------------------------------------------------------------------
enum class task_priority : uint8_t
{
    default_prority
}; // enum class task_priority

//-----------------------------------------------------------------------------------------------------------
enum class task_stack_request : uint8_t
{
    unknown,
    small_stack,
    huge_stack
}; // enum class task_stack_request

} // namespace xr::tasks
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
namespace xr::tasks::details
{

typedef void (*task_entry_function)(execution_context& context, pvoid user_data);
typedef void (*pool_task_destroy)(pvoid user_data);

struct task_desc
{
    //! task entry point
    task_entry_function task_func { nullptr };

    //! task destory entry pointe
    pool_task_destroy destroy_func { nullptr };

    //! task user data (task data context)
    pvoid user_data { nullptr };

    //! stack requirements for task
    task_stack_request required_stack { task_stack_request::unknown };

    //! priority for task
    task_priority priority { task_priority::default_prority };

#ifdef MT_INSTRUMENTED_BUILD
    pstr debug_id { nullptr };
    math::color_table debug_color { math::color_table::blue };
#endif

    constexpr task_desc() noexcept = default;

    constexpr task_desc(task_entry_function exec, pvoid user_data,
        task_stack_request stack_requirements, task_priority priority);

    constexpr task_desc(task_entry_function exec, pool_task_destroy destroy,
        pvoid user_data, task_stack_request stack_requirements, task_priority priority);

    constexpr bool is_valid() const;
};

constexpr task_desc::task_desc(task_entry_function func, pvoid ptr,
    task_stack_request stack_req, task_priority priority)
    : task_func { func }
    , user_data { ptr }
    , required_stack { stack_req }
    , priority { priority }
#ifdef MT_INSTRUMENTED_BUILD
    , debug_id { nullptr }
    , debug_color { Color::Blue }
#endif
{}

constexpr task_desc::task_desc(task_entry_function func, pool_task_destroy destroy,
    pvoid ptr, task_stack_request stack_req, task_priority priority)
    : task_func { func }
    , destroy_func { destroy }
    , user_data { ptr }
    , required_stack { stack_req }
    , priority { priority }
#ifdef MT_INSTRUMENTED_BUILD
    , debug_id { nullptr }
    , debug_color { Color::Blue }
#endif
{}

constexpr bool
task_desc::is_valid() const
{
    return (task_func != nullptr);
}

} // namespace xr::tasks::details
//-----------------------------------------------------------------------------------------------------------
