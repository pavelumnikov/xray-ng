// This file is a part of xray-ng engine
//

#include "io/memory_stream.h"
#include "corlib/memory/memory_functions.h"
#include "EASTL/algorithm.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//-----------------------------------------------------------------------------------------------------------
/**
*/
memory_stream::memory_stream(memory::base_allocator& alloc) noexcept
    : m_capacity { 0 }
    , m_size { 0 }
    , m_offset_position { 0 }
    , m_buffer { nullptr }
    , m_allocator { alloc }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
memory_stream::~memory_stream()
{
    if(this->is_open())
        this->close();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::can_read() const noexcept
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::can_write() const noexcept
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::can_seek() const noexcept
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t memory_stream::get_size() const noexcept
{
    auto const result = this->m_size;
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
size_t memory_stream::get_position() const noexcept
{
    auto const result = this->m_offset_position;
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void memory_stream::close()
{
    XR_DEBUG_ASSERTION(this->is_open());
    if(this->m_buffer != nullptr)
    {
        this->m_allocator.free_impl(this->m_buffer XR_DEBUG_PARAMETERS_DEFINITION);
        this->m_buffer = nullptr;
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::is_open() const
{
    return this->m_buffer != nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::write(memory::buffer_ref source, size_t bytes_to_write)
{
    XR_DEBUG_ASSERTION(source.is_valid());
    XR_DEBUG_ASSERTION(this->is_open());
    XR_DEBUG_ASSERTION(this->m_offset_position <= this->m_size);

    // If it's not enough capacity of stream, then reallocate
    if(!this->has_room(bytes_to_write))
        this->make_room(bytes_to_write);

    XR_DEBUG_ASSERTION((this->m_offset_position + bytes_to_write) <= this->m_capacity);
    memory::copy(this->m_buffer + this->m_offset_position,
        bytes_to_write, source.as_pointer<void*>(), bytes_to_write);

    this->m_offset_position += bytes_to_write;

    if(this->m_offset_position > this->m_size)
        this->m_size = this->m_offset_position;

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::read(memory::buffer_ref destination, size_t const bytes_to_read)
{
    XR_DEBUG_ASSERTION(destination.is_valid());
    XR_DEBUG_ASSERTION(this->is_open());
    XR_DEBUG_ASSERTION(this->m_offset_position <= this->m_size);

    auto const read_bytes = bytes_to_read <= this->m_size - this->m_offset_position ?
        bytes_to_read : this->m_size - this->m_offset_position;

    XR_DEBUG_ASSERTION((this->m_offset_position + read_bytes) <= this->m_size);

    if(read_bytes > 0)
    {
        memory::copy(destination.as_pointer<void*>(), read_bytes, 
            this->m_buffer + this->m_offset_position, read_bytes);

        this->m_offset_position += read_bytes;
    }

    return read_bytes > 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void memory_stream::seek(ptrdiff_t const offset, seek_origin const origin)
{
    assert(this->is_open());
    assert(this->m_offset_position <= this->m_size);

    switch(origin)
    {
        case seek_origin::begin:
            this->m_offset_position = offset;
            break;

        case seek_origin::current:
            this->m_offset_position += offset;
            break;

        case seek_origin::end:
            this->m_offset_position = this->m_size + offset;
            break;
    }

    this->m_offset_position = eastl::clamp<size_t>(
        this->m_offset_position, 0, this->m_size);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::eof() const noexcept
{
    assert(this->is_open());
    assert(this->m_offset_position <= this->m_size);
    return this->m_offset_position == this->m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
uint8_t* memory_stream::get_pointer_data() const noexcept
{
    assert(this->m_buffer != nullptr);
    return this->m_buffer;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void memory_stream::realloc(size_t sz)
{
    auto const new_buffer = reinterpret_cast<uintptr_t>(
        this->m_allocator.malloc_impl(sz
            XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("memory_stream::realloc")
            XR_DEBUG_PARAMETERS_DEFINITION));

#if _DEBUG
    auto const end_of_new_buffer = new_buffer + sz;
#endif

    XR_DEBUG_ASSERTION(new_buffer != 0);
    auto const new_size = sz < this->m_size ? sz : this->m_size;

    if(this->m_buffer != nullptr)
    {
#if _DEBUG
        XR_DEBUG_ASSERTION((new_buffer + new_size) < end_of_new_buffer);
#endif
        memory::copy(reinterpret_cast<void*>(new_buffer), 
            new_size, this->m_buffer, new_size);

        this->m_allocator.free_impl(
            this->m_buffer XR_DEBUG_PARAMETERS_DEFINITION);
    }

    this->m_buffer = reinterpret_cast<uint8_t*>(new_buffer);
    this->m_size = new_size;
    this->m_capacity = sz;

    if(this->m_offset_position > this->m_size)
        this->m_offset_position = this->m_size;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool memory_stream::has_room(size_t const sz) const
{
    auto const result = ((this->m_offset_position + sz) <= this->m_capacity);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void memory_stream::make_room(size_t const sz)
{
    assert((this->m_size + sz) > this->m_capacity);

    auto const one_dot_five_current_size = 
        this->m_capacity + (this->m_capacity >> 1);

    auto new_capacity = this->m_size + sz;

    if(one_dot_five_current_size > new_capacity)
        new_capacity = one_dot_five_current_size;

    assert(new_capacity > this->m_capacity);
    this->realloc(sz);
}

} // namespace xr::fs
//-----------------------------------------------------------------------------------------------------------