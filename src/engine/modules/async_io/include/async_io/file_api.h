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

#include "corlib/memory/buffer_ref.h"
#include "corlib/etl/string/fixed_string.h"
#include "corlib/etl/ptr.h"
#include "corlib/sys/win/handle.h"
#include "async_io/async_result.h"
#include "EASTL/string_view.h"

//------------------------------------------------------------------------------
namespace xr::memory
{
class base_allocator;
} // namespace xr::memory
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
namespace xr::async_io
{
// file path definiton
auto constexpr path_length = 256;
// file handle definition
using file_handle = sys::win::handle_wrapper<sys::win::handle_traits>;

using async_result_ptr = etl::shared_ptr<async_result>;
using path = etl::string::fixed_wstring<path_length>;
using path_view = eastl::wstring_view;

//------------------------------------------------------------------------------
// how stream must be opened.
enum class access_mode : uint8_t
{
    read_only, //!< Reading permissions, fail on writing
    write_only, //!< Writing permissions, fail on reading
    read_write, //!< Reading and writing permissions
    append, //!< Append access
}; // enum class access_mode

//------------------------------------------------------------------------------
// accessing pattern of current file.
enum class access_pattern : uint8_t
{
    random, //!< Random file access
    sequential, //!< Sequential file access
}; // enum class access_pattern

//------------------------------------------------------------------------------
class XR_NON_VIRTUAL file_api
{
public:
    virtual ~file_api()
    {};

    virtual file_handle create_file_descriptor(path_view name, 
        access_mode mode, access_pattern pattern) = 0;

    virtual void free_file_descriptor(file_handle handle) = 0;

    virtual async_result_ptr read_file_async(file_handle handle,
        memory::buffer_ref& buffer, size_t read, uint64_t offset = 0) = 0;

    virtual async_result_ptr write_file_async(file_handle handle,
        memory::buffer_ref& buffer, size_t write) = 0;

    virtual void cancel_async_io(async_result_ptr& ptr) = 0;
}; // class file_api

//------------------------------------------------------------------------------
/**
 */
void initialize_async_io(memory::base_allocator& alloc);

//------------------------------------------------------------------------------
/**
 */
void shutdown_async_io();

//------------------------------------------------------------------------------
/**
 */
file_api& current_file_api();

} // namespace xr::async_io
//------------------------------------------------------------------------------