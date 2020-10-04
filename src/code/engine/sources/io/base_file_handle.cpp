// This file is a part of xray-ng engine
//

#include "pch.h"
#include "base_file_handle.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle::base_file_handle(memory::base_allocator& alloc)
    : m_user_references { 0 }
    , m_allocator { alloc }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
base_file_handle::~base_file_handle()
{
    XR_DEBUG_ASSERTION_MSG(!m_user_references, "user reference count is not zero");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t base_file_handle::tell()
{
    return 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool base_file_handle::seek(ssize_t const pos)
{
    XR_UNREFERENCED_PARAMETER(pos);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool base_file_handle::seek_from_end(ssize_t const pos_relative_to_end)
{
    XR_UNREFERENCED_PARAMETER(pos_relative_to_end);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool base_file_handle::read(memory::buffer_ref ref, size_t bytes_to_read)
{
    XR_UNREFERENCED_PARAMETER(ref, bytes_to_read);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool base_file_handle::write(memory::buffer_ref ref, size_t bytes_to_write)
{
    XR_UNREFERENCED_PARAMETER(ref, bytes_to_write);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool base_file_handle::flush(bool const full_flush)
{
    XR_UNREFERENCED_PARAMETER(full_flush);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool base_file_handle::truncate(size_t new_size)
{
    XR_UNREFERENCED_PARAMETER(new_size);
    return false;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t base_file_handle::size()
{
    return 0;
}

XR_NAMESPACE_END(xr, filesystem)
//-----------------------------------------------------------------------------------------------------------