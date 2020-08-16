// This file is a part of xray-ng engine
//

#include "application.h"
#include "corlib/utils/static_vector.h"
#include "corlib/utils/string_conversion.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/tasks/task_system.h"
#include "corlib/sys/chrono.h"
#include "corlib/sys/thread.h"
#include "extension/context.h"
#include "async_io/file_api.h"
#include "../tasks/init_task.h"
#include "../tasks/tick_task.h"
#include "../config.h"
#include "../constants.h"

#if XRAY_PLATFORM_WINDOWS
#   include "win/win_main.h"
#   include "win/splash_screen.h"
#endif // XRAY_PLATFORM_WINDOWS

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, main, details)

static memory::uninitialized_reference<extension::context> the_main_context {};
static sys::tick the_last_update_time = 0;

//-----------------------------------------------------------------------------------------------------------
/**
 */
void create_window_from_config(memory::base_allocator& misc_allocator)
{
    uint32_t width = 640;
    uint32_t height = 480;

    const char* value = nullptr;
    if(find_config_value(constants::window_width, &value))
    {
        auto opt = utils::as_uint(value);
        if(opt.has_value())
            width = opt.value();
    }

    if(find_config_value(constants::window_height, &value))
    {
        auto opt = utils::as_uint(value);
        if(opt.has_value())
            height = opt.value();
    }

    (void)win::create_main_window(misc_allocator, width, height, false);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void pre_initialize_application(initialize_application_desc const& desc)
{
    create_window_from_config(desc.misc_allocator);
    tasks::initialize_tasks(desc.misc_allocator);
    memory::construct_reference(the_main_context, desc.misc_allocator);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void post_initialize_application()
{
    win::show_main_window();
    destroy_splash_screen();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void on_initialize_application(initialize_application_desc const& desc)
{
    constexpr uint32_t timeout_ms = 35000;
    tasks::scheduler& s = tasks::current_scheduler();

    task::init_task init { desc, the_main_context.ref() };
    s.run_async(tasks::task_group::get_default_group(), &init, 1);

    bool done_in_time = s.wait_group(tasks::task_group::get_default_group(), timeout_ms);
    XR_UNREFERENCED_PARAMETER(done_in_time);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool on_run_application(sys::tick start_point)
{
    XR_CONSTEXPR_CPP14_OR_STATIC_CONST sys::tick frame_ms = 33;
    XR_CONSTEXPR_CPP14_OR_STATIC_CONST uint32_t timeout_ms = 1500;

    sys::tick time_slice = the_last_update_time - start_point;
    if(the_last_update_time < start_point)
        time_slice = frame_ms;

    if(time_slice < frame_ms)
    {
        sys::yield(frame_ms - time_slice);
        time_slice = frame_ms;
    }

    task::tick_task tick { the_main_context.ref(), time_slice };
    tasks::scheduler& s = tasks::current_scheduler();
    s.run_async(tasks::task_group::get_default_group(), &tick, 1);

    bool done_in_time = s.wait_all(timeout_ms);
    XR_UNREFERENCED_PARAMETER(done_in_time);

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void on_shutdown_application()
{
    memory::destruct_reference(the_main_context);
    tasks::shutdown_tasks();
    shutdown_config();
}

XR_NAMESPACE_END(xr, game, main, details)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, main)

//-----------------------------------------------------------------------------------------------------------
/**
*/
void early_initialize_application(memory::base_allocator& misc_allocator)
{
    create_splash_screen(misc_allocator);
    initialize_config(misc_allocator);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void initialize_application(initialize_application_desc const& desc)
{
    details::pre_initialize_application(desc);
    details::on_initialize_application(desc);
    details::post_initialize_application();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void shutdown_application()
{
    details::on_shutdown_application();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool run_application()
{
    sys::tick start_point = sys::now_milliseconds();
    if(!win::run_main_loop())
        return false;

    return details::on_run_application(start_point);
}

XR_NAMESPACE_END(xr, game, main)
//-----------------------------------------------------------------------------------------------------------