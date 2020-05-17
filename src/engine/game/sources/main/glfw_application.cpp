// This file is a part of xray-ng engine
//

#include "glfw_application.h"
#include "win/splash_screen.h"
#include "GLFW/glfw3.h"
#include "corlib/utils/static_vector.h"
#include "corlib/utils/string_conversion.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/tasks/task_system.h"
#include "rendering_api/api.h"
#include "async_io/file_api.h"
#include "../modules/rendering_module.h"
#include "../config.h"
#include "../constants.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, main, details)

static memory::uninitialized_reference<modules::rendering_module> the_rendering_module;
static tasks::task_group the_initialization_group { tasks::task_group::invalid };
static tasks::task_group the_shutdown_group { tasks::task_group::invalid };

//-----------------------------------------------------------------------------------------------------------
/**
 */
void set_window_from_config(GLFWwindow* window)
{
    int width = 640;
    int height = 480;

    const char* value = nullptr;
    if(find_config_value(constants::window_width, &value))
    {
        auto opt = utils::as_int(value);
        if(opt.has_value())
            width = opt.value();
    }

    if(find_config_value(constants::window_height, &value))
    {
        auto opt = utils::as_int(value);
        if(opt.has_value())
            height = opt.value();
    }

    glfwSetWindowSize(window, width, height);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void pre_initialize_application(initialize_application_desc const& desc, GLFWwindow* window)
{
    set_window_from_config(window);

    // task system
    tasks::initialize_tasks(desc.misc_allocator);
    the_initialization_group = tasks::current_scheduler().create_group();
    XR_DEBUG_ASSERTION_MSG(the_initialization_group.is_valid(), "initialization tasks group is invalid");

    // asynchronous IO system
    async_io::initialize_async_io(desc.io_system_allocator);

    // rendering module
    XR_DEBUG_ASSERTION_MSG(!the_rendering_module.is_constructed(), "rendering module already inialized");
    memory::construct_reference(the_rendering_module, desc.rendering_allocator, window);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void post_initialize_application(GLFWwindow* window)
{
    glfwShowWindow(window);
    destroy_splash_screen();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void on_initialize_application(initialize_application_desc const& desc, GLFWwindow* window)
{
    constexpr uint32_t timeout_ms = 35000;
    tasks::scheduler& s = tasks::current_scheduler();

    rendering_api::descs::renderer_desc r_desc;
    r_desc.api = rendering_api::render_api::vulkan;
    r_desc.mode = rendering_api::gpu_mode::single;
    r_desc.target = rendering_api::shader_target::sm_5_1;
    r_desc.enable_gpu_based_validation = false;

    rendering_api::render_init_result result;
    rendering_api::initialize(desc.rendering_allocator, r_desc, result);
    XR_DEBUG_ASSERTION(result == rendering_api::render_init_result::success);

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

    // rendering module
    XR_DEBUG_ASSERTION_MSG(the_rendering_module.is_constructed(), "rendering module must be inialized");
    memory::destruct_reference(the_rendering_module);

    rendering_api::render_destroy_result result;
    rendering_api::shutdown(nullptr, result);
    XR_DEBUG_ASSERTION(result == rendering_api::render_destroy_result::success);

    async_io::shutdown_async_io();
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
void initialize_application(initialize_application_desc const& desc, GLFWwindow* window)
{
    details::pre_initialize_application(desc, window);
    details::on_initialize_application(desc, window);
    details::post_initialize_application(window);
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
void run_application()
{
    //constexpr uint32_t timeout_ms = 1500;

    //details::main_task first_task {};
    //tasks::scheduler& s = tasks::current_scheduler();
    //s.run_async(master_group, &first_task, 1);

    //bool done_in_time = s.wait_all(timeout_ms);
    //XR_UNREFERENCED_PARAMETER(done_in_time);
}

XR_NAMESPACE_END(xr, game, main)
//-----------------------------------------------------------------------------------------------------------