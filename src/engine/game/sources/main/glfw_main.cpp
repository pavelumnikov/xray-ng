// This file is a part of xray-ng engine
//

#include "glfw_application.h"
#include "corlib/threading/interlocked.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/memory_st_arena_allocator.h"
#include "corlib/memory/memory_crt_allocator.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/sys/thread.h"
#include "corlib/sys/chrono.h"
#include "GLFW/glfw3.h"
#include "../config.h"
#include "../constants.h"

#if defined(XRAY_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

using namespace xr;

//-----------------------------------------------------------------------------------------------------------
struct glfw_initializer final
{
    glfw_initializer(memory::base_allocator& alloc)
        : glfw_allocator { alloc }
    {
        GLFWallocator allocator =
        {
            malloc_v3_glfw,
            realloc_v3_glfw,
            free_v3_glfw,
            this
        };

        glfwInit(&allocator);
        glfwWindowHint(GLFW_VISIBLE, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(glfw_initializer);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(glfw_initializer);

    ~glfw_initializer()
    {
        glfwTerminate();
    }

private:
    memory::base_allocator& glfw_allocator;

    static void* malloc_v3_glfw(size_t size, void* args)
    {
        auto initializer = reinterpret_cast<glfw_initializer*>(args);

        XR_DEBUG_ASSERTION(initializer != nullptr);
        return initializer->glfw_allocator.malloc_impl(size
            XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("GLFW allocation")
            XR_DEBUG_PARAMETERS_DEFINITION);
    }

    static void* realloc_v3_glfw(void* ptr, size_t size, void* args)
    {
        auto initializer = reinterpret_cast<glfw_initializer*>(args);

        XR_DEBUG_ASSERTION(initializer != nullptr);
        return initializer->glfw_allocator.realloc_impl(ptr, size
            XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("GLFW allocation")
            XR_DEBUG_PARAMETERS_DEFINITION);
    }

    static void free_v3_glfw(void* ptr, void* args)
    {
        auto initializer = reinterpret_cast<glfw_initializer*>(args);
        XR_DEBUG_ASSERTION(initializer != nullptr);
        initializer->glfw_allocator.free_impl(ptr
            XR_DEBUG_PARAMETERS_DEFINITION);
    }
}; // struct glfw_initializer

//-----------------------------------------------------------------------------------------------------------
/**
 */
static int32_t early_engine_requirements_check()
{
    // may be it's better to check inside rendering subsystem, but it's better to check as
    // early as we can. it's bad idea to do such checks inside tasks, so this is placed here.
    if(glfwVulkanSupported() == GLFW_FALSE)
    {
        const char* error = nullptr;
        if(glfwGetError(&error) == GLFW_NO_ERROR)
            error = constants::vulkan_support_error_msg;
        if(MessageBoxA(nullptr, error, constants::xrayng_caption, MB_ICONSTOP | MB_OK) == MB_OK)
            return 1;
    }

    return 0;
}


//-----------------------------------------------------------------------------------------------------------
/**
 */
int main(int argc, char** argv)
{
    memory::crt_allocator misc_allocator {};
    game::main::early_initialize_application(misc_allocator);

    memory::st_arena_allocator glfw_allocator {};
    glfw_allocator.initialize(XR_MEGABYTES_TO_BYTES(1), XR_KILOBYTES_TO_BYTES(256), sys::current_thread_id());

    memory::mt_arena_allocator io_system_allocator {};
    io_system_allocator.initialize(XR_MEGABYTES_TO_BYTES(64), XR_MEGABYTES_TO_BYTES(16));

    memory::mt_arena_allocator rendering_subsystem_allocator {};
    rendering_subsystem_allocator.initialize(XR_MEGABYTES_TO_BYTES(256), XR_MEGABYTES_TO_BYTES(16));

    glfw_initializer glfw_init { glfw_allocator };
    int32_t result = early_engine_requirements_check();
    if(result) return result;

    auto const main_window = glfwCreateWindow(320, 240, constants::xrayng_caption, nullptr, nullptr);
    glfwSetInputMode(main_window, GLFW_STICKY_KEYS, GL_TRUE);

    game::main::initialize_application_desc desc
    {
        misc_allocator,
        io_system_allocator,
        rendering_subsystem_allocator
    };
    game::main::initialize_application(desc, main_window);

    bool exiting = false;
    while(!exiting)
    {
        // Use PeekMessage() so we can use idle time to render the scene. 
        bool const have_exit = !!glfwWindowShouldClose(main_window);

        if(have_exit)
        {
            exiting = true;
            break;
        }

        glfwPollEvents();
        game::main::run_application();
    }

    game::main::shutdown_application();
    glfwDestroyWindow(main_window);

    return result;
}

#if defined(XRAY_PLATFORM_WINDOWS)
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    XR_UNREFERENCED_PARAMETER(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return main(__argc, __argv);
}
#endif