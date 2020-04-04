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

#pragma once

#include "async_io/file_api.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/sys/thread.h"
#include "corlib/threading/spin_wait.h"
#include "corlib/etl/containers/std/deque.h"
#include "file_async_result_win32.h"

//------------------------------------------------------------------------------
namespace xr::async_io
{

//------------------------------------------------------------------------------
class file_api_win32 : public file_api
{
public:
    file_api_win32(memory::base_allocator& alloc);
    virtual ~file_api_win32();

    virtual file_handle create_file_descriptor(path_view name,
        access_mode mode, access_pattern pattern) override;

    virtual void free_file_descriptor(file_handle handle) override;

    virtual async_result_ptr read_file_async(file_handle handle,
        memory::buffer_ref& buffer, size_t read, uint64_t offset = 0) override;

    virtual async_result_ptr write_file_async(file_handle handle,
        memory::buffer_ref& buffer, size_t write) override;

    virtual void cancel_async_io(async_result_ptr& ptr) override;

private:
    using mutex = threading::spin_wait_fairness;
    static constexpr uint32_t max_io_threads = 2;

    static void async_io_func(void* arg);

    void add_request(async_result_ptr& ptr);
    bool try_deque_request(async_result_ptr& ptr);

    void process_request(async_result_ptr& ptr);
    void process_read_request(read_file_async_result_ptr& ptr);

    void initialize();
    void finalize();

    etl::containers::std::deque<async_result_ptr> m_async_requests;
    mutex m_async_requests_lock;
    sys::thread_handle m_thread_handles[max_io_threads];
    memory::base_allocator& m_allocator;
    volatile bool m_quit_status;
};

} // namespace xr::async_io
//------------------------------------------------------------------------------