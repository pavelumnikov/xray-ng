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

#include "async_io/file_api.h"
#include "file_async_result_win32.h"
#include "common.h"

namespace xr::async_io
{

file_handle create_file_descriptor(path_view name, access_mode mode, access_pattern pattern)
{
    DWORD access = 0;
    DWORD disposition = 0;
    DWORD share_mode = 0;
    DWORD flags_and_attributes = FILE_FLAG_OVERLAPPED;

    switch(mode)
    {
        case access_mode::read_only:
            access = GENERIC_READ;
            disposition = OPEN_EXISTING;
            share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;

        case access_mode::write_only:
            access = GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
            share_mode = FILE_SHARE_READ;
            break;

        case access_mode::read_write:
        case access_mode::append:
            access = GENERIC_READ | GENERIC_WRITE;
            disposition = OPEN_ALWAYS;
            share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
            break;

        default:
            XR_DEBUG_ASSERTION(false);
    }

    switch(pattern)
    {
        case access_pattern::random:
            flags_and_attributes |= FILE_FLAG_RANDOM_ACCESS;
            break;

        case access_pattern::sequential:
            flags_and_attributes |= FILE_FLAG_SEQUENTIAL_SCAN;
            break;

        default:
            XR_DEBUG_ASSERTION(false);
    }

    file_handle file { ::CreateFileW(name.data(), access, share_mode,
        nullptr, disposition, flags_and_attributes, nullptr) };

    // If we successfully created file session
    if(file.is_valid())
    {
        // in append mode, we need to seek to the end of the file
        if(access_mode::append == mode)
            SetFilePointer(file.get(), 0, nullptr, FILE_END);
    }

    return file;
}

void free_file_descriptor(file_handle handle)
{
    XR_DEBUG_ASSERTION_MSG(handle.is_valid(),
        "file handle must be valid before freeing");

    handle.close();
}

async_result_ptr read_file_async(file_handle handle, memory::buffer_ref& buffer, size_t read, uint64_t offset)
{
    common_io& io = get_common_io();
    memory::base_allocator& allocator = io.get_allocator();

    //
    auto result = etl::make_shared_ptr<read_file_async_result>(allocator, handle, buffer, read, offset);

    // 
    async_result_ptr ptr = eastl::static_pointer_cast<async_result>(result);
    io.add_request(ptr);

    return ptr;
}

async_result_ptr write_file_async(file_handle handle, memory::buffer_ref& buffer, size_t write)
{
    common_io& io = get_common_io();
    memory::base_allocator& allocator = io.get_allocator();
    
    //
    auto result = etl::make_shared_ptr<read_file_async_result>(allocator, handle, buffer, write, 0);

    // 
    async_result_ptr ptr = eastl::static_pointer_cast<async_result>(result);
    io.add_request(ptr);

    return ptr;
}

void cancel_async_io(async_result_ptr& ptr)
{
    auto file_async_ptr = eastl::static_pointer_cast<file_async_result>(ptr);
    file_async_ptr->set_current_status(async_status::cancelled);
}

} // namespace xr::async_io