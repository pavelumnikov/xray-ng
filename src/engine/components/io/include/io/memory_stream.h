// This file is a part of xray-ng engine
//

#pragma once

#include "io/stream.h"
#include "corlib/memory/memory_allocator_base.h"

//------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//------------------------------------------------------------------------------
/// <summary>
/// <c>memory_stream</c> is a stream class which writes to and reads from system RAM. 
/// Memory streams provide memory mapping for fast direct read/write access.
/// </summary>
class memory_stream final : public stream
{
    using super = stream;

public:
    /// <summary cref="memory_stream::memory_stream">
    /// Constructor.
    /// </summary>
    memory_stream(memory::base_allocator& alloc) noexcept;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(memory_stream);
    XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(memory_stream);

    /// <summary cref="memory_stream::~memory_stream">
    /// Destructor.
    /// </summary>
    virtual ~memory_stream();

    /// <summary cref="memory_stream::can_read">
    /// Check if we can read from stream.
    /// </summary>
    /// <returns>True if the stream supports reading.</returns>
    bool can_read() const noexcept override;

    /// <summary cref="memory_stream::can_write">
    /// Check if we can write to the stream.
    /// </summary>
    /// <returns>True if the stream supports writing.</returns>
    bool can_write() const noexcept override;

    /// <summary cref="memory_stream::can_seek">
    /// Check if current stream supports seeking.
    /// </summary>
    /// <returns>True if the stream supports seeking.</returns>
    bool can_seek() const noexcept override;

    /// <summary cref="memory_stream::get_size">
    /// Get the size of the stream in bytes.
    /// </summary>
    /// <returns>size_t of stream.</returns>
    size_t get_size() const noexcept override;

    /// <summary cref="memory_stream::get_position">
    /// Get the current position of the read/write cursor.
    /// </summary>
    /// <returns>Position of stream in memory.</returns>
    size_t get_position() const noexcept override;

    /// <summary cref="memory_stream::close">
    /// Close the stream.
    /// </summary>
    void close() override;

    bool is_open() const override;

    /// <summary cref="memory_stream::write">
    /// Directly write to the stream.
    /// </summary>
    /// <param name="source">A pointer to the stream.</param>
    /// <param name="bytes_to_write">size_t of data.</param>
    bool write(memory::buffer_ref source, size_t bytes_to_write) override;

    /// <summary cref="memory_stream::read">
    /// Directly read from the stream.
    /// </summary>
    /// <param name="destination">A pointer to the data where we read from.</param>
    /// <param name="bytes_to_read">size_t of data.</param>
    /// <returns>Read data in bytes.</returns>
    bool read(memory::buffer_ref destination, size_t bytes_to_read) override;

    /// <summary cref="memory_stream::seek">
    /// Seek in stream.
    /// </summary>
    /// <param name="offset">Offset in bytes.</param>
    /// <param name="origin">Origin in data.</param>
    void seek(ptrdiff_t offset, seek_origin origin) override;

    /// <summary cref="memory_stream::eof">
    /// End of reading.
    /// </summary>
    /// <returns>True if end-of-stream reached, otherwise false.</returns>
    bool eof() const noexcept override;

    /// <summary cref="memory_stream::get_pointer_data">
    /// Get pointer to the data of memory stream.
    /// </summary>
    /// <returns>Pointer to the data.</returns>
    uint8_t* get_pointer_data() const noexcept;

private:
    /// <summary cref="memory_stream::realloc">
    /// Re-allocate stream buffer.
    /// </summary>
    void realloc(size_t sz);

    /// <summary cref="memory_stream::has_room">
    /// Do we have room for data writing?
    /// </summary>
    /// <returns>True if capacity is good, otherwise false.</returns>
    bool has_room(size_t sz) const;

    /// <summary cref="memory_stream::make_room">
    /// Make free room for data to write.
    /// </summary>
    /// <param name="sz">Memory size to add/re-allocate.</param>
    void make_room(size_t sz);

    //!< Every reallocation adds this or required size of buffer
    static constexpr size_t initial_size = 256;

    size_t m_capacity; //!< Capacity of memory stream
    size_t m_size; //!< size_t of written data
    size_t m_offset_position; //!< Position
    uint8_t* m_buffer; //!< Buffer of written data
    memory::base_allocator& m_allocator; //!< Allocator that holds all allocations here
}; // class memory_stream

XR_NAMESPACE_END(xr, io)
//------------------------------------------------------------------------------