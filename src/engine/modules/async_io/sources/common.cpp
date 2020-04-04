/*

    Copyright (c) 2020, Pavel Umnikov
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

#include "async_io/io_system.h"
#include "corlib/memory/uninitialized_reference.h"
#include "common.h"
#include "file_async_result_win32.h"
#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "corlib/threading/scoped_lock.h"

namespace xr::async_io
{

static memory::uninitialized_reference<common_io> common_io_system;
common_io& get_common_io()
{
    XR_DEBUG_ASSERTION_MSG(common_io_system.is_constructed(),
        "async io system must be initialized");
    return common_io_system.ref();
}

common_io::common_io(memory::base_allocator& alloc)
    : m_async_requests { memory::proxy::eastl_proxy_allocator { alloc } }
    , m_async_requests_lock {}
    , m_thread_handles {}
    , m_allocator { alloc }
    , m_quit_status { false }
{
    initialize();
}

common_io::~common_io()
{
    finalize();
}

void common_io::add_request(async_result_ptr& ptr)
{
    threading::scoped_lock lock { m_async_requests_lock };
    m_async_requests.push_back(ptr);
}

bool common_io::try_deque_request(async_result_ptr& ptr)
{
    threading::scoped_lock lock { m_async_requests_lock };
    if(m_async_requests.empty())
        return false;

    ptr = eastl::move(m_async_requests.front());
    m_async_requests.pop_front();
    return true;
}

void common_io::process_request(async_result_ptr& ptr)
{
    if(ptr->is_file_io())
    {
        if(ptr->is_read_request())
        {
            auto read_file_async = eastl::static_pointer_cast<read_file_async_result>(ptr);
            process_read_request(read_file_async);
        }
    }
}

void common_io::process_read_request(read_file_async_result_ptr& ptr)
{
    XR_DEBUG_ASSERTION_MSG(ptr, "Invalid read file request");
    HANDLE handle = ptr->get_file_handle().get();

    memory::buffer_ref& buffer = ptr->get_buffer_ref();
    XR_DEBUG_ASSERTION_MSG(buffer.is_valid(), "Invalid input buffer");

    size_t read_size = ptr->get_read_size();
    XR_DEBUG_ASSERTION_MSG(buffer.length() >= read_size,
        "Input buffer size lower than request read size");

    size_t offset = ptr->get_offset();

    DWORD bytes_read = 0;
    (void)ReadFile(handle, buffer.as_pointer<void*>(), static_cast<DWORD>(read_size), &bytes_read, nullptr);
}

void common_io::initialize()
{
    for(size_t i = 0; i < eastl::size(m_thread_handles); ++i)
    {
        m_thread_handles[i] = sys::spawn_thread(m_allocator, async_io_func, this, L"io_worker_thread", sys::thread_priority::high, 1_mb, static_cast<uint32_t>(i));
    }
}

void common_io::finalize()
{
    threading::atomic_store_rel(m_quit_status, true);
    bool result = sys::wait_threads(m_thread_handles, eastl::size(m_thread_handles));
}

void common_io::async_io_func(void* arg)
{
    common_io* ptr = reinterpret_cast<common_io*>(arg);
    XR_DEBUG_ASSERTION_MSG(ptr, "async io thread invalid argument");
    while(!threading::atomic_fetch_acq(ptr->m_quit_status))
    {
        async_result_ptr async_ptr {};
        if(ptr->try_deque_request(async_ptr))
        {
            ptr->process_request(async_ptr);
        }
        else
        {
            sys::yield(15);
        }
    }
}

void initialize_async_io(memory::base_allocator& alloc)
{
    XR_DEBUG_ASSERTION_MSG(!common_io_system.is_constructed(),
        "async io system already initialized");

    memory::construct_reference(common_io_system, alloc);
}

void shutdown_async_io()
{
    XR_DEBUG_ASSERTION_MSG(common_io_system.is_constructed(),
        "async io system must be already initialized");

    memory::destruct_reference(common_io_system);
}

} // namespace xr::async_io