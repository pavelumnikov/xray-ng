// This file is a part of xray-ng engine
//

#include "file_api_win32.h"
#include "file_async_result_win32.h"
#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "corlib/threading/scoped_lock.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, async_io)

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_api_win32::file_api_win32(memory::base_allocator& alloc)
    : m_async_requests(memory::proxy::eastl_proxy_allocator(alloc))
    , m_async_requests_lock()
    , m_thread_handles()
    , m_allocator(alloc)
    , m_quit_status(false)
{
    initialize();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_api_win32::~file_api_win32()
{
    finalize();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_handle file_api_win32::create_file_descriptor(path_view name, access_mode mode, access_pattern pattern)
{
    DWORD access = 0;
    DWORD disposition = 0;
    DWORD share_mode = 0;
    DWORD flags_and_attributes = FILE_FLAG_OVERLAPPED;

    switch(mode)
    {
        case access_mode::read_only:
            access = GENERIC_READ;
            disposition = OPEN_EXISTING;
            share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;

        case access_mode::write_only:
            access = GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
            share_mode = FILE_SHARE_READ;
            break;

        case access_mode::read_write:
        case access_mode::append:
            access = GENERIC_READ | GENERIC_WRITE;
            disposition = OPEN_ALWAYS;
            share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;

        default:
            XR_DEBUG_ASSERTION(false);
    }

    switch(pattern)
    {
        case access_pattern::random:
            flags_and_attributes |= FILE_FLAG_RANDOM_ACCESS;
            break;

        case access_pattern::sequential:
            flags_and_attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            break;

        default:
            XR_DEBUG_ASSERTION(false);
    }

    file_handle file(::CreateFileW(name.data(), access, share_mode,
        nullptr, disposition, flags_and_attributes, nullptr));

    // If we successfully created file session
    if(file.is_valid())
    {
        // in append mode, we need to seek to the end of the file
        if(access_mode::append == mode)
            SetFilePointer(file.get(), 0, nullptr, FILE_END);
    }

    return file;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::free_file_descriptor(file_handle handle)
{
    XR_DEBUG_ASSERTION_MSG(handle.is_valid(),
        "file handle must be valid before freeing");

    handle.close();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_result_ptr file_api_win32::read_file_async(file_handle handle, 
    memory::buffer_ref& buffer, size_t read, uint32_t offset)
{
    auto result = utils::make_shared_ptr<read_file_async_result>(m_allocator, handle, buffer, read, offset);
    async_result_ptr ptr = eastl::static_pointer_cast<async_result>(result);
    add_request(ptr);
    return ptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_result_ptr file_api_win32::write_file_async(file_handle handle, 
    memory::buffer_ref& buffer, size_t write)
{
    auto result = utils::make_shared_ptr<read_file_async_result>(m_allocator, handle, buffer, write, 0);
    async_result_ptr ptr = eastl::static_pointer_cast<async_result>(result);
    add_request(ptr);
    return ptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::cancel_async_io(async_result_ptr& ptr)
{
    auto file_async_ptr = eastl::static_pointer_cast<file_async_result>(ptr);
    file_async_ptr->set_current_status(async_status::cancelled);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::add_request(async_result_ptr& ptr)
{
    threading::scoped_lock lock { m_async_requests_lock };
    m_async_requests.push_back(ptr);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_api_win32::try_deque_request(async_result_ptr& ptr)
{
    threading::scoped_lock lock { m_async_requests_lock };
    if(m_async_requests.empty())
        return false;

    ptr = eastl::move(m_async_requests.front());
    m_async_requests.pop_front();
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::process_request(async_result_ptr& ptr)
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

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::process_read_request(read_file_async_result_ptr& ptr)
{
    XR_DEBUG_ASSERTION_MSG(ptr, "Invalid read file request");
    HANDLE handle = ptr->get_file_handle().get();

    memory::buffer_ref& buffer = ptr->get_buffer_ref();
    XR_DEBUG_ASSERTION_MSG(buffer.is_valid(), "Invalid input buffer");

    size_t read_size = ptr->get_read_size();
    XR_DEBUG_ASSERTION_MSG(buffer.length() >= read_size,
        "Input buffer size lower than request read size");

    LPOVERLAPPED overlapped = &ptr->get_overlapped();
    BOOL status = ReadFileEx(handle, buffer.as_pointer<void*>(),
        static_cast<DWORD>(read_size), overlapped, &file_async_result::request_func);

    if(status)
    {
        ptr->set_current_status(async_status::error);
        return;
    }
    else
    {
        ptr->set_current_status(async_status::processing);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::initialize()
{
    for(size_t i = 0; i < eastl::size(m_thread_handles); ++i)
    {
        m_thread_handles[i] = sys::spawn_thread(async_io_func, this, L"io_worker_thread",
            sys::thread_priority::high, XR_MEGABYTES_TO_BYTES(1), static_cast<uint32_t>(i));
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_api_win32::finalize()
{
    threading::atomic_store_rel(m_quit_status, true);
    bool result = sys::wait_threads(m_thread_handles, eastl::size(m_thread_handles));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t file_api_win32::async_io_func(void* arg)
{
    file_api_win32* ptr = reinterpret_cast<file_api_win32*>(arg);
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

    return 0;
}

XR_NAMESPACE_END(xr, async_io)
//-----------------------------------------------------------------------------------------------------------