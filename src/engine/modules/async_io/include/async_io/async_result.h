// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/threading/interlocked.h"

namespace xr::async_io
{

enum class async_status : uint8_t
{
    prepared,
    processing,
    done,
    error,
    cancelled
}; // enum class async_status

enum class async_request : uint8_t
{
    unknown,
    read,
    write,
    append
}; // enum class async_request

enum class async_type : uint8_t
{
    file_io,
    network_io
}; // enum class async_type

class XR_NON_VIRTUAL async_result
{
public:
    virtual ~async_result()
    {};

    async_type request_io_type() const noexcept;
    async_request request_type() const noexcept;
    async_status current_status() const noexcept;

    signalling_bool is_prepared() const noexcept;
    signalling_bool is_processing() const noexcept;
    signalling_bool is_done() const noexcept;
    signalling_bool is_cancelled() const noexcept;

    signalling_bool is_file_io() const noexcept;
    signalling_bool is_networking_io() const noexcept;

    signalling_bool is_write_request() const noexcept;
    signalling_bool is_read_request() const noexcept;

protected:
    constexpr async_result(async_type type, async_request req) noexcept;
    const async_type m_async_type;
    const async_request m_async_request;
    volatile async_status m_async_status;
}; // class async_result

constexpr
async_result::async_result(async_type type, async_request req) noexcept
    : m_async_type { type }
    , m_async_request { req }
    , m_async_status { async_status::prepared }
{}

inline async_type
async_result::request_io_type() const noexcept
{
    return m_async_type;
}

inline async_request
async_result::request_type() const noexcept
{
    return m_async_request;
}

inline async_status
async_result::current_status() const noexcept
{
    return threading::atomic_fetch_acq(m_async_status);
}

inline signalling_bool
async_result::is_prepared() const noexcept
{
    return current_status() == async_status::prepared;
}

inline signalling_bool
async_result::is_processing() const noexcept
{
    return current_status() == async_status::processing;
}

inline signalling_bool
async_result::is_done() const noexcept
{
    return current_status() == async_status::done;
}

inline signalling_bool
async_result::is_cancelled() const noexcept
{
    return current_status() == async_status::done;
}

inline signalling_bool
async_result::is_file_io() const noexcept
{
    return request_io_type() == async_type::file_io;
}

inline signalling_bool
async_result::is_networking_io() const noexcept
{
    return request_io_type() == async_type::network_io;
}

inline signalling_bool
async_result::is_read_request() const noexcept
{
    return request_type() == async_request::read;
}

inline signalling_bool
async_result::is_write_request() const noexcept
{
    return request_type() == async_request::write;
}

} // namespace xr::async_io