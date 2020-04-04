/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#include "glfw_application.h"
#include "corlib/threading/interlocked.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/memory_st_arena_allocator.h"
#include "corlib/memory/memory_crt_allocator.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/sys/thread.h"
#include "corlib/sys/chrono.h"
#include "GLFW/glfw3.h"

#if defined(XRAY_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

using namespace xr;

//------------------------------------------------------------------------------
#if defined(XRAY_PLATFORM_WINDOWS)
struct ole_initializer final
{
    ole_initializer()
    {
#if XR_WITH_EDITOR
        // Sciter needs it for drag-n-drop, etc.
        (void)OleInitialize(nullptr);
#endif // defined(XRAY_PLATFORM_WINDOWS)
    }

    ~ole_initializer()
    {
#if XR_WITH_EDITOR
        OleUninitialize();
#endif // defined(XRAY_PLATFORM_WINDOWS)
    }

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(ole_initializer);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(ole_initializer);
}; // struct ole_initializer
#endif // defined(XRAY_PLATFORM_WINDOWS)

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/**
*/
int main(int argc, char** argv)
{
#if defined(XRAY_PLATFORM_WINDOWS)
    ole_initializer ole_init {};
#endif // defined(XRAY_PLATFORM_WINDOWS)

    memory::crt_allocator misc_allocator {};
    memory::st_arena_allocator glfw_allocator {};
    glfw_allocator.initialize(1_mb, 256_kb, sys::current_thread_id());
    memory::mt_arena_allocator io_system_allocator {};
    io_system_allocator.initialize(64_mb, 16_mb);

    glfw_initializer glfw_init { glfw_allocator };
    auto const main_window = glfwCreateWindow(320, 240, "xray-ng", nullptr, nullptr);
    game::main::initialize_application(misc_allocator, io_system_allocator, main_window);

    bool exiting = false;
    while(!exiting)
    {
        // Use PeekMessage() so we can use idle time to render the scene. 
        auto const have_exit = !!glfwWindowShouldClose(main_window);

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
}

#if defined(XRAY_PLATFORM_WINDOWS)
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    return main(__argc, __argv);
}
#endif