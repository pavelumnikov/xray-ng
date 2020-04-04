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

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/signalling_bool.h"
#include "corlib/sys/chrono.h"
#include "corlib/etl/function_pointer.h"
#include "EASTL/string_view.h"
#include "EASTL/optional.h"

#if defined(XRAY_PLATFORM_WINDOWS)
#   include "corlib/sys/win/min_windows.h"
#endif // defined(XRAY_PLATFORM_WINDOWS)

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
enum class thread_priority
{
    low,
    medium, //!< Normal priority, e.g. default priority
    high
}; // enum class thread_priority

#if defined(XRAY_PLATFORM_WINDOWS)
using thread_handle = win::HANDLE;
using thread_id = uint32_t;
auto constexpr unknown_thread_handle = nullptr;
// See http://blogs.msdn.com/b/oldnewthing/archive/2004/02/23/78395.aspx
thread_id constexpr invalid_thread_id = 0;
#else
#error "Thread handle is not defined for this platform!"
#endif

using thread_function = etl::function_pointer_t<void, void*>;

//------------------------------------------------------------------------------
/**
*/
thread_handle spawn_thread(memory::base_allocator& alloc,
    thread_function function, void* const arg,
    eastl::wstring_view debug_thread_name, thread_priority priority,
    size_t stack_size, eastl::optional<uint32_t> hardware_thread = {});

//------------------------------------------------------------------------------
/**
*/
signalling_bool wait_threads(thread_handle const* threads, size_t threads_count,
    bool wait_all_threads = true, eastl::optional<sys::tick> timeout = {});

//------------------------------------------------------------------------------
/**
*/
void detach_thread(thread_handle thread);

//------------------------------------------------------------------------------
/**
*/
void yield(eastl::optional<sys::tick> timeout = {});

//------------------------------------------------------------------------------
/**
*/
uint32_t core_count();

//------------------------------------------------------------------------------
/**
*/
thread_id current_thread_id();

//------------------------------------------------------------------------------
/**
*/
uint32_t current_thread_affinity();

//------------------------------------------------------------------------------
/**
*/
eastl::wstring_view get_current_thread_name();

} // namespace xr::sys
//------------------------------------------------------------------------------