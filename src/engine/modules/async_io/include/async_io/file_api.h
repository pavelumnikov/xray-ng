// This file is a part of xray-ng engine
//

#pragma once

#include "async_io/async_result.h"
#include "corlib/memory/buffer_ref.h"
#include "corlib/utils/buffer_string.h"
#include "corlib/utils/string_view.h"
#include "corlib/utils/ptr.h"
#include "corlib/sys/win/handle.h"
#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, async_io)

// file path definiton
size_t XR_CONSTEXPR_CPP14_OR_CONST path_length = 256;

// file handle definition
typedef sys::win::handle_wrapper<sys::win::handle_traits> file_handle;

typedef utils::shared_ptr<async_result> async_result_ptr;
typedef utils::wstring_view path_view;

//-----------------------------------------------------------------------------------------------------------
// how stream must be opened.
enum class access_mode : uint8_t
{
    read_only, //!< Reading permissions, fail on writing
    write_only, //!< Writing permissions, fail on reading
    read_write, //!< Reading and writing permissions
    append, //!< Append access
}; // enum class access_mode

//-----------------------------------------------------------------------------------------------------------
// accessing pattern of current file.
enum class access_pattern : uint8_t
{
    random, //!< Random file access
    sequential, //!< Sequential file access
}; // enum class access_pattern

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
/**
 */
void initialize_async_io(memory::base_allocator& alloc);

//-----------------------------------------------------------------------------------------------------------
/**
 */
void shutdown_async_io();

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_api& current_file_api();

XR_NAMESPACE_END(xr, async_io)
//-----------------------------------------------------------------------------------------------------------
