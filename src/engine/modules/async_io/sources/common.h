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

namespace xr::async_io
{

class common_io
{
public:
    common_io(memory::base_allocator& alloc);
    ~common_io();

    void add_request(async_result_ptr& ptr);
    memory::base_allocator& get_allocator();

private:
    using mutex = threading::spin_wait_fairness;
    static constexpr uint32_t max_io_threads = 2;

    static void async_io_func(void* arg);
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

inline memory::base_allocator&
common_io::get_allocator()
{
    return m_allocator;
}

common_io& get_common_io();

} // namespace xr::async_io