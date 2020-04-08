// This file is a part of xray-ng engine
//

#pragma once

#include "async_io/async_result.h"
#include "async_io/file_api.h"
#include <Windows.h>

//------------------------------------------------------------------------------
namespace xr::async_io::details
{

//------------------------------------------------------------------------------
struct OVERLAPPEDEX : public OVERLAPPED
{
    pvoid used_data;
}; // struct OVERLAPPEDEX

} // namespace xr::async_io::details
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::async_io
{

//------------------------------------------------------------------------------
class XR_NON_VIRTUAL file_async_result : public async_result
{
public:
    virtual ~file_async_result()
    {};

    void set_current_status(async_status status) noexcept;

    file_handle& get_file_handle();
    details::OVERLAPPEDEX& get_overlapped();

protected:
    friend class file_api_win32;
    file_async_result(async_request request, file_handle& handle, uint32_t offset) noexcept;
    static void request_func(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

private:
    details::OVERLAPPEDEX m_overlapped;
    file_handle& m_file_handle;
}; // class file_async_result

//------------------------------------------------------------------------------
/**
 */
inline file_handle&
file_async_result::get_file_handle()
{
    return m_file_handle;
}

//------------------------------------------------------------------------------
/**
 */
inline details::OVERLAPPEDEX&
file_async_result::get_overlapped()
{
    return m_overlapped;
}


//------------------------------------------------------------------------------
class XR_NON_VIRTUAL read_file_async_result : public file_async_result
{
public:
    read_file_async_result(file_handle& handle, memory::buffer_ref& buffer,
        size_t read, uint32_t offset);

    virtual ~read_file_async_result()
    {};

    memory::buffer_ref& get_buffer_ref();
    size_t get_read_size() const;

private:
    //! memory buffer to use with
    memory::buffer_ref& m_buffer_ref;
    const size_t m_read_size;
}; // class read_file_async_result
using read_file_async_result_ptr = etl::shared_ptr<read_file_async_result>;

//------------------------------------------------------------------------------
/**
 */
inline memory::buffer_ref& 
read_file_async_result::get_buffer_ref()
{
    return m_buffer_ref;
}

//------------------------------------------------------------------------------
/**
 */
inline size_t
read_file_async_result::get_read_size() const
{
    return m_read_size;
}

} // namespace xr::async_io
//------------------------------------------------------------------------------