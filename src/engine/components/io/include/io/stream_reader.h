// This file is a part of xray-ng engine
//

#pragma once

#include "io/stream.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

class stream_reader
{
public:
    stream_reader(stream_reader&& mv) noexcept = default;
    stream_reader& operator=(stream_reader&& mv) noexcept = default;

    stream_reader(stream_reader const&) = delete;
    stream_reader& operator=(stream_reader const&) = delete;

    /// <summary cref="stream_reader::~stream_reader">
    /// Destructor.
    /// </summary>
    virtual ~stream_reader();

    /// <summary cref="stream_reader::GetStream">
    /// Get input stream.
    /// </summary>
    /// <returns>Reference to the input stream.</returns>
    const stream& get_input() const noexcept;

    /// <summary cref="stream_reader::Eof">
    /// Check end of file.
    /// </summary>
    /// <returns>True if it's end of file, otherwise false.</returns>
    bool eof() const noexcept;

    /// <summary cref="stream_reader::Close">
    /// Close stream from reading.
    /// </summary>
    void close() const;

    /// <summary cref="stream_reader::IsOpen">
    /// Check if we have opened this stream.
    /// </summary>
    /// <returns>True if stream is opened, otherwise false.</returns>
    bool is_open() const noexcept;

protected:
    /// <summary cref="stream_reader::stream_reader">
    /// Constructor.
    /// </summary>
    explicit stream_reader(stream& s) noexcept;

    stream& m_stream; //!< Stream to read from
};

inline bool stream_reader::is_open() const noexcept
{
    auto const result = this->m_stream.is_open();
    return result;
}

XR_NAMESPACE_END(xr, io)
