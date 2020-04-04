/*

Copyright (c) 2020, Pavel Umnikov
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE.

*/

#pragma once

#include "async_io/async_result.h"
#include "async_io/file_api.h"
#include <Windows.h>

namespace xr::async_io
{

class XR_NON_VIRTUAL file_async_result : public async_result
{
public:
    virtual ~file_async_result()
    {};

    void set_current_status(async_status status) noexcept;
    file_handle& get_file_handle();

protected:
    file_async_result(async_request request, file_handle& handle) noexcept;

private:
    file_handle& m_file_handle;
}; // class file_async_result

inline file_handle&
file_async_result::get_file_handle()
{
    return m_file_handle;
}


class XR_NON_VIRTUAL read_file_async_result : public file_async_result
{
public:
    read_file_async_result(file_handle& handle, memory::buffer_ref& buffer, size_t read, size_t offset);
    virtual ~read_file_async_result()
    {};

    memory::buffer_ref& get_buffer_ref();
    size_t get_read_size() const;
    size_t get_offset() const;

private:
    memory::buffer_ref& m_buffer_ref;
    const size_t m_read_size;
    const size_t m_offset;
};
using read_file_async_result_ptr = etl::shared_ptr<read_file_async_result>;

inline memory::buffer_ref& 
read_file_async_result::get_buffer_ref()
{
    return m_buffer_ref;
}

inline size_t
read_file_async_result::get_read_size() const
{
    return m_read_size;
}

inline size_t
read_file_async_result::get_offset() const
{
    return m_offset;
}

} // namespace xr::async_io