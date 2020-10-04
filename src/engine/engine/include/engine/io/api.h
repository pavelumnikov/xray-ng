// This file is a part of xray-ng engine
//

#pragma once

#include "engine/linkage.h"
#include "corlib/utils/string_view.h"
#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

class base_file_handle;

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle* open_read(memory::base_allocator& alloc, utils::wstring_view filename, bool allow_write);

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle* open_read_no_buffering(memory::base_allocator& alloc, utils::wstring_view filename, bool allow_write);

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle* open_write(memory::base_allocator& alloc, utils::wstring_view filename, bool append, bool allow_read);

//-----------------------------------------------------------------------------------------------------------
/**
 */
void close_file_handle(base_file_handle* handle);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool directory_exists(utils::wstring_view path);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool create_directory(utils::wstring_view path);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool delete_directory(utils::wstring_view path);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool move(utils::wstring_view from, utils::wstring_view to);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool set_read_only_status(utils::wstring_view path, bool const new_read_only_status);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool is_read_only(utils::wstring_view path);

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------
