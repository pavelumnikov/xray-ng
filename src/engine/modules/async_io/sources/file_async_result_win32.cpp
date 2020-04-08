// This file is a part of xray-ng engine
//

#include "file_async_result_win32.h"

//------------------------------------------------------------------------------
namespace xr::async_io
{

//------------------------------------------------------------------------------
/**
 */
file_async_result::file_async_result(async_request request, file_handle& handle, uint32_t offset) noexcept
    : async_result { async_type::file_io, request }
    , m_file_handle { handle }
{
    m_overlapped.Offset = offset;
}

//------------------------------------------------------------------------------
/**
 */
void file_async_result::set_current_status(async_status status) noexcept
{
    threading::atomic_store_rel(m_async_status, status);
}

//------------------------------------------------------------------------------
/**
 */
void file_async_result::request_func(DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    // TODO: implement me
}


//------------------------------------------------------------------------------
/**
 */
read_file_async_result::read_file_async_result(file_handle& handle,
    memory::buffer_ref& buffer, size_t read, uint32_t offset)
    : file_async_result { async_request::read, handle, offset }
    , m_buffer_ref { buffer }
    , m_read_size { read }
{}

} // namespace xr::async_io
//------------------------------------------------------------------------------