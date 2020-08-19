// This file is a part of xray-ng engine
//

#include "pch.h"
#include "io/file_stream.h"
#include "base_file_handle.h"
#include "io/api.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_stream::file_stream(class base_file_handle* handle, bool owning)
    : m_file_handle { handle }
    , m_is_owning_handle { owning }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
file_stream::~file_stream()
{
    if(this->is_open())
        this->close();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::can_read() const
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::can_write() const
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::can_seek() const
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t file_stream::get_size() const
{
    XR_DEBUG_ASSERTION(this->is_open());
    return m_file_handle->size();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
size_t file_stream::get_position() const
{
    XR_DEBUG_ASSERTION(this->is_open());
    return m_file_handle->tell();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_stream::close()
{
    XR_DEBUG_ASSERTION(this->is_open());
    if(this->is_owning_file_handle())
        close_file_handle(this->m_file_handle);

    this->m_file_handle = nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::is_open() const
{
    return this->m_file_handle != nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::write(memory::buffer_ref source, size_t bytes_to_write)
{
    XR_DEBUG_ASSERTION(source.is_valid());
    XR_DEBUG_ASSERTION(this->is_open());
    
    return m_file_handle->write(memory::buffer_ref { source }, bytes_to_write);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::read(memory::buffer_ref destination, size_t bytes_to_read)
{
    XR_DEBUG_ASSERTION(destination.is_valid());
    XR_DEBUG_ASSERTION(this->is_open());
    return m_file_handle->read(destination, bytes_to_read);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_stream::seek(ptrdiff_t offset, seek_origin origin)
{
    XR_DEBUG_ASSERTION(this->is_open());

    if(origin == seek_origin::end)
        m_file_handle->seek_from_end(offset);
    else
        m_file_handle->seek(offset);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void file_stream::flush()
{
    XR_DEBUG_ASSERTION(this->is_open());
    m_file_handle->flush();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool file_stream::eof() const
{
    XR_DEBUG_ASSERTION(this->is_open());

    auto const pos = m_file_handle->tell();
    auto const size = m_file_handle->size();
    // NOTE: THE '>=' IS NOT A mistake!!!
    return pos >= size;
}

XR_NAMESPACE_END(xr, io)
//-----------------------------------------------------------------------------------------------------------