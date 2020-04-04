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
#include "win/splash_screen.h"
#include "GLFW/glfw3.h"
#include "corlib/etl/containers/static_vector.h"
#include "corlib/threading/interlocked.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/tasks/task_system.h"
#include "async_io/io_system.h"
#include "corlib/sys/thread.h"
#include <Windows.h>

//------------------------------------------------------------------------------
namespace xr::game::main::details
{

//------------------------------------------------------------------------------
/**
*/
void pre_initialize_application(memory::base_allocator& misc_allocator,
    memory::base_allocator& io_system_allocator)
{
    create_splash_screen(misc_allocator);
    tasks::initialize_tasks(misc_allocator);
    async_io::initialize_async_io(io_system_allocator);
}

//------------------------------------------------------------------------------
/**
*/
void post_initialize_application(GLFWwindow* window)
{
    glfwShowWindow(window);
    destroy_splash_screen();
}

//------------------------------------------------------------------------------
/**
*/
void on_initialize_application()
{
}

//------------------------------------------------------------------------------
/**
*/
void on_shutdown_application()
{
    async_io::shutdown_async_io();
    tasks::shutdown_tasks();
}

} // namespace xr::game::main::details

//------------------------------------------------------------------------------
namespace xr::game::main
{

//------------------------------------------------------------------------------
/**
*/
void initialize_application(
    memory::base_allocator& misc_allocator,
    memory::base_allocator& io_system_allocator,
    GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    details::pre_initialize_application(misc_allocator, io_system_allocator);
    details::on_initialize_application();
    details::post_initialize_application(window);
}

//------------------------------------------------------------------------------
/**
*/
void shutdown_application()
{
    details::on_shutdown_application();
}

//------------------------------------------------------------------------------
/**
*/
void run_application()
{
    //constexpr uint32_t timeout_ms = 1500;

    //sys::thread_id main_thread = sys::current_thread_id();
    //tasks::task_group master_group = tasks::task_group::get_default_group();

    //details::main_task first_task {};
    //tasks::scheduler& s = tasks::current_scheduler();
    //s.run_async(master_group, &first_task, 1);

    //bool done_in_time = s.wait_all(timeout_ms);
    //XR_UNREFERENCED_PARAMETER(done_in_time);
}

} // namespace xr::paranoia
//------------------------------------------------------------------------------