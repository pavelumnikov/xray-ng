// This file is a part of xray-ng engine
//

#pragma once

#include "async_io/file_api.h"
#include "corlib/memory/memory_allocator_base.h"
#include "corlib/sys/thread.h"
#include "corlib/threading/spin_wait.h"
#include "corlib/utils/deque.h"
#include "file_async_result_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, async_io)

//-----------------------------------------------------------------------------------------------------------
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
    static XR_CONSTEXPR_CPP14_OR_CONST uint32_t max_io_threads = 2;

    static uint32_t async_io_func(void* arg);

    void add_request(async_result_ptr& ptr);
    bool try_deque_request(async_result_ptr& ptr);

    void process_request(async_result_ptr& ptr);
    void process_read_request(read_file_async_result_ptr& ptr);

    void initialize();
    void finalize();

    utils::deque<async_result_ptr> m_async_requests;
    mutex m_async_requests_lock;
    sys::thread_handle m_thread_handles[max_io_threads];
    memory::base_allocator& m_allocator;
    volatile bool m_quit_status;
};

XR_NAMESPACE_END(xr, async_io)
//-----------------------------------------------------------------------------------------------------------
