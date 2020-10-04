// This file is a part of xray-ng engine
//

#include "pch.h"
#include "async_file_handler_win32.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
XR_CONSTEXPR_CPP14_OR_STATIC_CONST size_t the_default_read_size = XR_MEGABYTES_TO_BYTES(1);

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_file_handle::async_file_handle(memory::base_allocator& alloc, sys::win::file_handle_wrapper handle)
    : base_file_handle { alloc }
    , m_overlapped {}
    , m_handle { eastl::move(handle) }
    , m_file_size { 0 }
    , m_file_pos { 0 }
{
    open();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
async_file_handle::~async_file_handle()
{
    close();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t async_file_handle::tell()
{
    XR_DEBUG_ASSERTION(is_valid());
    return m_file_pos;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::seek(ssize_t const pos)
{
    XR_DEBUG_ASSERTION(is_valid());
    XR_DEBUG_ASSERTION(pos >= 0);

    m_file_pos = pos;
    update_overlaped_pos();

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::seek_from_end(ssize_t const pos_relative_to_end)
{
    XR_DEBUG_ASSERTION(pos_relative_to_end <= 0);
    // position is negative so this is actually subtracting
    return seek(m_file_size + pos_relative_to_end);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::read(memory::buffer_ref ref, size_t bytes_to_read)
{
    XR_DEBUG_ASSERTION(is_valid());

    ssize_t num_total_read = 0;
    void* ptr = ref.as_pointer<void*>();

    do
    {
        DWORD num_read = 0;
        DWORD num_bytes_to_read = eastl::min(static_cast<uint32_t>(bytes_to_read), UINT32_MAX);

        if(!ReadFile(m_handle.get(), ptr, num_bytes_to_read, &num_read, &m_overlapped))
        {
            DWORD error_code = GetLastError();
            if(error_code != ERROR_IO_PENDING) // read failed
                return false;

            // wait for the read to complete

            num_read = 0;
            if(!GetOverlappedResult(m_handle.get(), &m_overlapped, &num_read, 1)) // read failed
                return false;
        }

        bytes_to_read -= num_bytes_to_read;
        ptr = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(ptr) + num_bytes_to_read);
        num_total_read += num_read;

        // update where we are in the file

        m_file_pos += num_read;
        update_overlaped_pos();

        // early out as a failure case if we did not read all of the bytes that we expected to read.
        if(num_bytes_to_read != num_read)
            return false;
    } while(bytes_to_read > 0);

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::write(memory::buffer_ref ref, size_t bytes_to_write)
{
    XR_DEBUG_ASSERTION(is_valid());

    ssize_t num_total_written = 0;
    void* ptr = ref.as_pointer<void*>();

    do
    {
        DWORD num_written = 0;
        DWORD num_bytes_to_write = eastl::min(static_cast<uint32_t>(bytes_to_write), UINT32_MAX);

        if(!WriteFile(m_handle.get(), ptr, num_bytes_to_write, &num_written, &m_overlapped))
        {
            DWORD error_code = GetLastError();
            if(error_code != ERROR_IO_PENDING) // write failed
                return false;

            // wait for the read to complete

            num_written = 0;
            if(!GetOverlappedResult(m_handle.get(), &m_overlapped, &num_written, 1)) // write failed
                return false;
        }

        bytes_to_write -= num_bytes_to_write;
        ptr = reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(ptr) + num_bytes_to_write);
        num_total_written += num_written;

        // update where we are in the file

        m_file_pos += num_written;
        update_overlaped_pos();

        // early out as a failure case if we did not write all of the bytes that we expected to write.
        if(num_bytes_to_write != num_written)
            return false;
    } while(bytes_to_write > 0);

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::flush(bool const full_flush)
{
    XR_DEBUG_ASSERTION(is_valid());
    return FlushFileBuffers(m_handle.get()) != 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::truncate(size_t new_size)
{
    // SetEndOfFile isn't an overlapped operation, so we need to call updated_non_overlapped_pos
    // after seeking to ensure that the file pointer is in the correct place.

    if(seek(new_size) && updated_non_overlapped_pos() && SetEndOfFile(m_handle.get()) != 0)
    {
        update_file_size();
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t async_file_handle::size()
{
    XR_DEBUG_ASSERTION(is_valid());
    return m_file_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_file_handle::open()
{
    if(is_valid()) update_file_size();
    memory::zero_struct(&m_overlapped);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_file_handle::close()
{
    if(m_handle.is_valid())
    {
        m_handle.close();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_file_handle::update_overlaped_pos()
{
    ULARGE_INTEGER LI;
    LI.QuadPart = m_file_pos;
    m_overlapped.Offset = LI.LowPart;
    m_overlapped.OffsetHigh = LI.HighPart;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool async_file_handle::updated_non_overlapped_pos()
{
    LARGE_INTEGER LI;

#if defined(XR_PLATFORM_64BIT)
    LI.QuadPart = static_cast<LONGLONG>(m_file_pos);
#else
    LI.LowPart = static_cast<DWORD>(m_file_pos);
#endif // defined(XR_PLATFORM_64BIT)

    return SetFilePointer(m_handle.get(), LI.LowPart, &LI.HighPart, FILE_BEGIN) != INVALID_SET_FILE_POINTER;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void async_file_handle::update_file_size()
{
#if defined(XR_PLATFORM_64BIT)
    LARGE_INTEGER LI;
    GetFileSizeEx(m_handle.get(), &LI);
    m_file_size = static_cast<ssize_t>(LI.QuadPart);
#else
    DWORD size = GetFileSize(m_handle.get(), nullptr);
    m_file_size = static_cast<ssize_t>(size);
#endif // defined(XR_PLATFORM_64BIT)
}

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------