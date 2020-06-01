// This file is a part of xray-ng engine
//

#include "application.h"
#include "corlib/utils/static_vector.h"
#include "corlib/utils/string_conversion.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/tasks/task_system.h"
#include "extension/context.h"
#include "async_io/file_api.h"
#include "../tasks/init_task.h"
#include "../config.h"
#include "../constants.h"

#if XRAY_PLATFORM_WINDOWS
#   include "win/win_main.h"
#   include "win/splash_screen.h"
#endif // XRAY_PLATFORM_WINDOWS

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, main, details)

static memory::uninitialized_reference<extension::context> the_main_context {};
static tasks::task_group the_initialization_group { tasks::task_group::invalid };
static tasks::task_group the_shutdown_group { tasks::task_group::invalid };

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

    // task system
    tasks::initialize_tasks(desc.misc_allocator);
    the_initialization_group = tasks::current_scheduler().create_group();
    XR_DEBUG_ASSERTION_MSG(the_initialization_group.is_valid(), "initialization tasks group is invalid");
    // create main extensibility context
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
    s.run_async(the_initialization_group, &init, 1);

    bool done_in_time = s.wait_group(the_initialization_group, timeout_ms);
    XR_UNREFERENCED_PARAMETER(done_in_time);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void on_shutdown_application()
{
    the_shutdown_group = tasks::current_scheduler().create_group();
    XR_DEBUG_ASSERTION_MSG(the_shutdown_group.is_valid(), "shutdown tasks group is invalid");

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
    static XR_CONSTEXPR_CPP14_OR_CONST uint32_t timeout_ms = 1500;
    if(!win::run_main_loop())
        return false;

    //details::main_task first_task {};
    tasks::scheduler& s = tasks::current_scheduler();
    //s.run_async(master_group, &first_task, 1);

    bool done_in_time = s.wait_all(timeout_ms);
    XR_UNREFERENCED_PARAMETER(done_in_time);

    return true;
}

XR_NAMESPACE_END(xr, game, main)
//-----------------------------------------------------------------------------------------------------------