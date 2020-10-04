// This file is a part of xray-ng engine
//

#pragma once

#include "engine/io/stream.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
class stream_writer
{
public:
    stream_writer(stream_writer&& mv) noexcept = default;
    stream_writer& operator=(stream_writer&& mv) noexcept = default;

    stream_writer(stream_writer const&) = delete;
    stream_writer& operator=(stream_writer const&) = delete;

    /// <summary cref="stream_writer::~stream_writer">
    /// Destructor.
    /// </summary>
    virtual ~stream_writer();

    /// <summary cref="stream_writer::GetStream">
    /// Get input stream.
    /// </summary>
    /// <returns>Reference to the input stream.</returns>
    const stream& get_input() const;

    /// <summary cref="stream_writer::Eof">
    /// Check end of file.
    /// </summary>
    /// <returns>True if it's end of file, otherwise false.</returns>
    bool eof() const;

    /// <summary cref="stream_writer::Close">
    /// Close stream from writing.
    /// </summary>
    void close() const;

    /// <summary cref="stream_writer::IsOpen">
    /// Check if we have opened this stream.
    /// </summary>
    /// <returns>True if stream is opened, otherwise false.</returns>
    bool is_open() const;

private:
    /// <summary cref="stream_writer::stream_writer">
    /// Constructor.
    /// </summary>
    /// <param name="rhs">Reference to the input stream to write to.</param>
    explicit stream_writer(stream& rhs);

    stream& m_stream;
};

inline bool stream_writer::is_open() const
{
    auto const result = this->m_stream.is_open();
    return result;
}

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------
