// This file is a part of xray-ng engine
//

#include "pch.h"
#include "corlib/memory/allocator_macro.h"
#include "corlib/memory/memory_functions.h"
#include "async_buffered_file_handler_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
XR_CONSTEXPR_CPP14_OR_STATIC_CONST size_t the_default_buffer_size = XR_KILOBYTES_TO_BYTES(64);

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_buffered_file_handle::async_buffered_file_handle(memory::base_allocator& alloc, sys::win::file_handle_wrapper handle)
    : base_file_handle{ alloc }
    , m_allocator { alloc }
    , m_overlapped {}
    , m_handle { eastl::move(handle) }
    , m_file_size { 0 }
    , m_file_pos { 0 }
    , m_buffer_size { the_default_buffer_size }
    , m_overlapped_pos { 0 }
    , m_serialize_pos { 0 }
    , m_buffers { nullptr, nullptr }
    , m_is_eof { false }
    , m_has_read_outstanding { false }
    , m_serialize_buffer { the_reading_buffer }
    , m_stream_buffer { the_streaming_buffer }
    , m_current_async_read_buffer { the_reading_buffer }
{
    open();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_buffered_file_handle::~async_buffered_file_handle()
{
    close();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t async_buffered_file_handle::tell()
{
    XR_DEBUG_ASSERTION(is_valid());
    return m_file_pos;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::seek(ssize_t const pos)
{
    XR_DEBUG_ASSERTION(is_valid());
    XR_DEBUG_ASSERTION(pos <= m_file_size);

    ssize_t delta = pos - m_file_pos;
    if(!delta) return true; // Same place so no work to do

    // no matter what, we need to wait for the current async read to finish since
    // we most likely need to issue a new one.
    if(!wait_for_async_read())
        return false;

    m_file_pos = pos;

    // if the requested location is not within our current serialize buffer,
    // we need to start the whole read process over.

    bool within_serialize_buffer = (delta < 0 && (m_serialize_pos - abs(delta) >= 0)) ||
        (delta > 0 && ((delta + m_serialize_pos) < m_buffer_size));

    if(!within_serialize_buffer)
    {
        // reset our EOF tracking and let the read handle setting it if need be.
        m_is_eof = false;

        // not within our buffer so start a new async read on the serialize buffer.
        m_overlapped_pos = pos;

        copy_overlapped_position();
        m_current_async_read_buffer = m_serialize_buffer;
        m_serialize_pos = 0;

        start_async_serialize_buffer_read();
    }
    else
    {
        // still within the serialize buffer so just update the position.
        m_serialize_pos += delta;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::seek_from_end(ssize_t const pos_relative_to_end)
{
    XR_DEBUG_ASSERTION(is_valid());
    XR_DEBUG_ASSERTION(pos_relative_to_end <= 0);
    return seek(m_file_size + pos_relative_to_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::read(memory::buffer_ref ref, size_t bytes_to_read)
{
    XR_DEBUG_ASSERTION(is_valid());
    // if zero were requested, quit (some calls like to do zero sized reads).
    if(!bytes_to_read) return false;

    if(m_current_async_read_buffer == m_serialize_buffer)
    {
        // first async read after either construction or a seek.
        if(!wait_for_async_read())
            return false;

        start_async_stream_buffer_read();
    }

    XR_DEBUG_ASSERTION(ref.is_valid());
    void* ptr = ref.as_pointer<void*>();

    // while there is data to copy
    while(bytes_to_read > 0)
    {
        XR_DEBUG_ASSERTION(m_buffer_size >= m_serialize_pos);
        size_t diff = size_t(m_buffer_size) - size_t(m_serialize_pos);
        // figure out how many bytes we can read from the serialize buffer.
        size_t num_to_copy = eastl::min(bytes_to_read, diff);

        if(m_file_pos + num_to_copy > m_file_size)
            return false;

        // see if we are at the end of the serialize buffer or not
        if(num_to_copy > 0)
        {
            memory::copy(ptr, ref.length(), &m_buffers[m_serialize_buffer][m_serialize_pos], num_to_copy);

            // update the internal positions

            m_serialize_pos += ssize_t(num_to_copy);
            XR_DEBUG_ASSERTION(m_serialize_pos <= m_buffer_size);

            m_file_pos += num_to_copy;
            XR_DEBUG_ASSERTION(m_file_pos <= m_file_size);

            bytes_to_read -= num_to_copy;
            ptr = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(ptr) + num_to_copy);
        }
        else
        {
            // we've crossed the buffer boundary and now need to make sure the stream buffer read is done.
            if(!wait_for_async_read())
                return false;

            exchange_buffers();
            start_async_stream_buffer_read();
        }
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::write(memory::buffer_ref ref, size_t bytes_to_write)
{
    XR_UNREFERENCED_PARAMETER(ref, bytes_to_write);
    XR_DEBUG_ASSERTION_MSG(false, "this is an async reader only and doesn't support writing");
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::flush(bool const full_flush)
{
    // reader only, so don't need to support flushing

    XR_UNREFERENCED_PARAMETER(full_flush);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::truncate(size_t new_size)
{
    // reader only, so don't need to support truncation

    XR_UNREFERENCED_PARAMETER(new_size);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t async_buffered_file_handle::size()
{
    XR_DEBUG_ASSERTION(is_valid());
    return m_file_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::open()
{
    LARGE_INTEGER LI;
    GetFileSizeEx(m_handle.get(), &LI);
    m_file_size = LI.QuadPart;

    allocate_buffers();

    // zero the overlapped structure, or this might become invalid
    memory::zero_struct(&m_overlapped);

    // kick off the first async read
    start_async_serialize_buffer_read();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::close()
{
    // can't free the buffers or close the file if a read is outstanding
    wait_for_async_read();
    close_internal();
    free_buffers();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::allocate_buffers()
{
    m_buffers[the_reading_buffer] = XR_ALLOCATE_OBJECT_ARRAY_T(m_allocator, int8_t, m_buffer_size, "the_reading_buffer");
    m_buffers[the_streaming_buffer] = XR_ALLOCATE_OBJECT_ARRAY_T(m_allocator, int8_t, m_buffer_size, "the_streaming_buffer");

    XR_DEBUG_ASSERTION_MSG(m_buffers[the_reading_buffer] != nullptr, "failed to allocate serialization buffer");
    XR_DEBUG_ASSERTION_MSG(m_buffers[the_streaming_buffer] != nullptr, "failed to allocate streaming buffer");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::free_buffers()
{
    XR_DEALLOCATE_MEMORY_T(m_allocator, m_buffers[the_reading_buffer]);
    XR_DEALLOCATE_MEMORY_T(m_allocator, m_buffers[the_streaming_buffer]);

    m_buffers[the_reading_buffer] = nullptr;
    m_buffers[the_streaming_buffer] = nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::exchange_buffers()
{
    m_stream_buffer ^= 1;
    m_serialize_buffer ^= 1;
    // we are now at the beginning of the serialize buffer
    m_serialize_pos = 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::close_internal()
{
    if(m_handle.is_valid())
    {
        m_handle.close();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::copy_overlapped_position()
{
    ULARGE_INTEGER LI;
    LI.QuadPart = m_overlapped_pos;
    m_overlapped.Offset = LI.LowPart;
    m_overlapped.OffsetHigh = LI.HighPart;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::update_file_offset_after_read(uint32_t amount_read)
{
    m_has_read_outstanding = false;
    m_overlapped_pos += amount_read;
    // update the overlapped structure since it uses this for where to read from
    copy_overlapped_position();
    if(m_overlapped_pos >= size_t(amount_read))
        m_is_eof = true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_buffered_file_handle::wait_for_async_read()
{
    // check for already being at EOF because we won't issue a read
    if(m_is_eof || !m_has_read_outstanding)
        return true;

    DWORD num_read = 0;
    if(GetOverlappedResult(m_handle.get(), &m_overlapped, &num_read, 1) != 0)
    {
        update_file_offset_after_read(num_read);
        return true;
    }
    else if(GetLastError() == ERROR_HANDLE_EOF)
    {
        m_is_eof = true;
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_buffered_file_handle::start_async_read(uint8_t buffer_to_read_into)
{
    if(m_is_eof)
        return;

    m_has_read_outstanding = true;
    m_current_async_read_buffer = buffer_to_read_into;

    DWORD num_read = 0;
    DWORD num_bytes_to_read = eastl::min(static_cast<uint32_t>(m_buffer_size), UINT32_MAX);
    if(!ReadFile(m_handle.get(), m_buffers[buffer_to_read_into], num_bytes_to_read, &num_read, &m_overlapped))
    {
        DWORD error_code = GetLastError();
        if(error_code != ERROR_IO_PENDING)
        {
            m_is_eof = true;
            m_has_read_outstanding = false;
        }
    }
    else
    {
        // read completed immediately
        update_file_offset_after_read(num_read);
    }
}

XR_NAMESPACE_END(xr, filesystem)
//-----------------------------------------------------------------------------------------------------------