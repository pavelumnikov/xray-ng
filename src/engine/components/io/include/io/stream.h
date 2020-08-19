// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "corlib/memory/buffer_ref.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//-----------------------------------------------------------------------------------------------------------
enum class seek_origin : uint8_t
{
    begin, //!< Seek file from beginning
    current, //!< Seek file from current position
    end, //!< Seek file from the end
}; // enum class seek_origin

//-----------------------------------------------------------------------------------------------------------
class XR_NON_VIRTUAL stream
{
public:
    virtual ~stream() = default;

    /// Check if we can read from stream.
    virtual bool can_read() const;

    /// Check if we can write to the stream.
    virtual bool can_write() const;

    /// Check if current stream supports seeking.
    virtual bool can_seek() const;

    /// Get the size of the stream in bytes.
    virtual size_t get_size() const;

    /// Get the current position of the read/write cursor.
    virtual size_t get_position() const;

    /// Close the stream.
    virtual void close();

    /// Check if stream has been opened.
    virtual bool is_open() const;

    /// Directly write to the stream.
    virtual bool write(memory::buffer_ref source, size_t bytes_to_write);

    /// Directly read from the stream.
    virtual bool read(memory::buffer_ref destination, size_t bytes_to_read);

    /// Seek in stream.
    virtual void seek(ptrdiff_t offset, seek_origin origin);

    /// Flush unsaved data.
    virtual void flush();

    /// End of reading.
    virtual bool eof() const;
}; // class stream

XR_NAMESPACE_END(xr, io)
//-----------------------------------------------------------------------------------------------------------
