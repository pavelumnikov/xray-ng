// This file is a part of xray-ng engine
//

#pragma once

#include "base_file_handle.h"
#include "corlib/sys/win/handle.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL async_file_handle : public base_file_handle
{
public:
    XR_IO_API async_file_handle(memory::base_allocator& alloc, sys::win::file_handle_wrapper handle);
    virtual XR_IO_API ~async_file_handle();
    virtual XR_IO_API size_t tell() override;
    virtual XR_IO_API bool seek(ssize_t const pos) override;
    virtual XR_IO_API bool seek_from_end(ssize_t const pos_relative_to_end) override;
    virtual XR_IO_API bool read(memory::buffer_ref ref, size_t bytes_to_read) override;
    virtual XR_IO_API bool write(memory::buffer_ref ref, size_t bytes_to_write) override;
    virtual XR_IO_API bool flush(bool const full_flush = false) override;
    virtual XR_IO_API bool truncate(size_t new_size) override;
    virtual XR_IO_API size_t size() override;

private:
    void open();
    void close();

    bool is_valid() const;
    void update_overlaped_pos();
    bool updated_non_overlapped_pos();
    void update_file_size();

    OVERLAPPED m_overlapped; //!< The overlapped IO struct to use for determining async state
    sys::win::file_handle_wrapper m_handle; //!< The file handle to operate on
    ssize_t m_file_size; //!< The size of the file that is being read
    ssize_t m_file_pos; //!< Overall position in the file and buffers combined
}; // class async_file_handle

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool async_file_handle::is_valid() const
{
    return m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE;
}

XR_NAMESPACE_END(xr, io)
//-----------------------------------------------------------------------------------------------------------
