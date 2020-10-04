// This file is a part of xray-ng engine
//

#pragma once

#include "engine/io/stream.h"
#include "corlib/utils/string_view.h"
#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
class base_file_handle;

//-----------------------------------------------------------------------------------------------------------
// stream class to which offers read/write access to filesystem files.
class file_stream final : public stream
{
public:
    file_stream(class base_file_handle* handle, bool owning = true);
    virtual ~file_stream();

    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(file_stream);
    XR_DECLARE_DELETE_COPY_ASSIGNMENT(file_stream);

    /// Get is current file stream owns file handle
    constexpr bool is_owning_file_handle() const noexcept;

    /// Check if we can read from stream.
    bool can_read() const override;

    /// Check if we can write to the stream.
    bool can_write() const override;

    /// Check if current stream supports seeking.
    bool can_seek() const override;

    /// Get the size of the stream in bytes.
    size_t get_size() const override;

    /// Get the current position of the read/write cursor.
    size_t get_position() const override;

    /// Close the stream.
    void close() override;

    /// Check if stream has been opened.
    bool is_open() const override;

    /// Directly write to the stream.
    bool write(memory::buffer_ref source, size_t bytes_to_write) override;

    /// Directly read from the stream.
    bool read(memory::buffer_ref destination, size_t bytes_to_read) override;

    /// Seek in stream.
    void seek(ptrdiff_t offset, seek_origin origin) override;

    /// Flush unsaved data.
    void flush() override;

    /// End of reading.
    bool eof() const override;

protected:
    bool m_is_owning_handle;
    class base_file_handle* m_file_handle;
}; // class file_stream

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr bool file_stream::is_owning_file_handle() const noexcept
{
    return m_is_owning_handle;
}

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------
