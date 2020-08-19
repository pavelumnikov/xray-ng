// This file is a part of xray-ng engine
//

#pragma once

#include "base_file_handle.h"
#include "corlib/sys/win/handle.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL async_buffered_file_handle : public base_file_handle
{
public:
    XR_IO_API async_buffered_file_handle(memory::base_allocator& alloc, sys::win::file_handle_wrapper handle);
    virtual XR_IO_API ~async_buffered_file_handle();
    virtual XR_IO_API size_t tell() override;
    virtual XR_IO_API bool seek(ssize_t const pos) override;
    virtual XR_IO_API bool seek_from_end(ssize_t const pos_relative_to_end) override;
    virtual XR_IO_API bool read(memory::buffer_ref ref, size_t bytes_to_read) override;
    virtual XR_IO_API bool write(memory::buffer_ref ref, size_t bytes_to_write) override;
    virtual XR_IO_API bool flush(bool const full_flush = false) override;
    virtual XR_IO_API bool truncate(size_t new_size) override;
    virtual XR_IO_API size_t size() override;

private:
    static XR_CONSTEXPR_CPP14_OR_CONST uint8_t the_reading_buffer = 0;
    static XR_CONSTEXPR_CPP14_OR_CONST uint8_t the_streaming_buffer = 1;

    void open();
    void close();
    bool is_valid() const;

    void allocate_buffers();
    void free_buffers();
    void exchange_buffers();

    void close_internal();
    void copy_overlapped_position();
    void update_file_offset_after_read(uint32_t amount_read);
    bool wait_for_async_read();

    void start_async_read(uint8_t buffer_to_read_into);
    void start_async_stream_buffer_read();
    void start_async_serialize_buffer_read();

    memory::base_allocator& m_allocator; //!< Allocator associated with buffers
    OVERLAPPED m_overlapped; //!< The overlapped IO struct to use for determining async state
    sys::win::file_handle_wrapper m_handle; //!< The file handle to operate on
    ssize_t m_file_size; //!< The size of the file that is being read
    ssize_t m_file_pos; //!< Overall position in the file and buffers combined
    ssize_t m_buffer_size; //!< The size of the buffers in bytes
    ssize_t m_serialize_pos; //!< Where we are in the serialize buffer
    size_t m_overlapped_pos; //!< Overall position in the file as the OverlappedIO struct understands it
    int8_t* m_buffers[2]; //!< These are the two buffers used for reading the file asynchronously
    bool m_is_eof; //!< Used to track whether the last read reached the end of the file or not. Reset when a Seek happens
    bool m_has_read_outstanding; //!< Whether there's a read outstanding or not
    uint8_t m_serialize_buffer; //!< The current index of the buffer that we are serializing from
    uint8_t m_stream_buffer; //!< The current index of the streaming buffer for async reading into
    uint8_t m_current_async_read_buffer; //!< Tracks which buffer has the async read outstanding (0 = first read after create/seek, 1 = streaming buffer)
}; // class async_file_handle

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool async_buffered_file_handle::is_valid() const
{
    return m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void async_buffered_file_handle::start_async_stream_buffer_read()
{
    start_async_read(the_streaming_buffer);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void async_buffered_file_handle::start_async_serialize_buffer_read()
{
    start_async_read(the_reading_buffer);
}

XR_NAMESPACE_END(xr, filesystem)
//-----------------------------------------------------------------------------------------------------------