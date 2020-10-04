// This file is a part of xray-ng engine
//

#include "pch.h"
#include "async_buffered_file_handler_win32.h"
#include "async_file_handler_win32.h"
#include "corlib/memory/allocator_macro.h"
#include "corlib/sys/win/handle.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
/**
 */
static wchar_t* windows_normalized_filename(utils::wstring_view filename)
{
    XR_CONSTEXPR_CPP14_OR_STATIC_CONST uint8_t max_nested_buffers = 4;

    static thread_local uint8_t index = 0;
    static thread_local wchar_t the_localized_filename[max_nested_buffers][MAX_PATH] = {};

    wchar_t* current_buffer = the_localized_filename[index];
    index = (index + 1) & max_nested_buffers;

    // TODO: implement normalization for Windows platform
    wcscpy(current_buffer, eastl::data(filename));
    return current_buffer;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
static HANDLE create_platform_handle_read(utils::wstring_view filename, bool allow_write)
{
    DWORD access = GENERIC_READ;
    DWORD flags = FILE_SHARE_READ | (allow_write ? FILE_SHARE_WRITE : 0);
    DWORD create = OPEN_EXISTING;
    DWORD attributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED;
    return CreateFileW(windows_normalized_filename(filename), access, flags, nullptr, create, attributes, nullptr);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle* open_read(memory::base_allocator& alloc, utils::wstring_view filename, bool allow_write)
{
    sys::win::file_handle_wrapper handle { create_platform_handle_read(filename, allow_write) };
    if(handle != INVALID_HANDLE_VALUE)
        return XR_ALLOCATE_OBJECT_T(alloc, async_buffered_file_handle, "open_read")(alloc, eastl::move(handle));

    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle* open_read_no_buffering(memory::base_allocator& alloc, utils::wstring_view filename, bool allow_write)
{
    sys::win::file_handle_wrapper handle { create_platform_handle_read(filename, allow_write) };
    if(handle != INVALID_HANDLE_VALUE)
        return XR_ALLOCATE_OBJECT_T(alloc, async_file_handle, "open_read_no_buffering")(alloc, eastl::move(handle));

    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle* open_write(memory::base_allocator& alloc, utils::wstring_view filename, bool append, bool allow_read)
{
    DWORD access = GENERIC_WRITE | (allow_read ? GENERIC_READ : 0);
    DWORD flags = allow_read ? FILE_SHARE_READ : 0;
    DWORD create = append ? OPEN_ALWAYS : CREATE_ALWAYS;
    DWORD attributes = FILE_ATTRIBUTE_NORMAL;

    sys::win::file_handle_wrapper handle
    { 
        CreateFileW(windows_normalized_filename(filename), access, flags, nullptr, create, attributes, nullptr)
    };

    if(handle.is_valid())
    {
        async_file_handle* h = XR_ALLOCATE_OBJECT_T(alloc, async_file_handle, "open_write")(alloc, eastl::move(handle));
        if(h) h->seek_from_end(0);
        return h;
    }

    return nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void close_file_handle(base_file_handle* handle)
{
    XR_DEBUG_ASSERTION_MSG(handle != nullptr, "Invalid file handle passed!");
    memory::base_allocator& allocator = handle->allocator();
    XR_DEALLOCATE_MEMORY_T(allocator, handle);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool directory_exists(utils::wstring_view path)
{
    // empty path means relative position, relative path always exists
    if(path.empty()) return true;

    DWORD result = GetFileAttributesW(windows_normalized_filename(path));
    return (result != UINT32_MAX && (result & FILE_ATTRIBUTE_DIRECTORY));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool create_directory(utils::wstring_view path)
{
    return CreateDirectoryW(windows_normalized_filename(path), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool delete_directory(utils::wstring_view path)
{
    RemoveDirectoryW(windows_normalized_filename(path));

    DWORD last_error = GetLastError();
    bool const succeeded = !directory_exists(path);

    if(!succeeded)
        SetLastError(last_error);

    return succeeded;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool move(utils::wstring_view from, utils::wstring_view to)
{
    return !!MoveFileW(windows_normalized_filename(from), windows_normalized_filename(to));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool set_read_only_status(utils::wstring_view path, bool const new_read_only_status)
{
    return !!SetFileAttributesW(windows_normalized_filename(path),
        new_read_only_status ? FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_NORMAL);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool is_read_only(utils::wstring_view path)
{
    DWORD result = GetFileAttributesW(windows_normalized_filename(path));
    if(result != UINT32_MAX)
        return !!(result & FILE_ATTRIBUTE_READONLY);

    return false;
}

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------