// This file is a part of xray-ng engine
//

#include "io/stream_reader.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, io)

//-----------------------------------------------------------------------------------------------------------
/**
*/
stream_reader::stream_reader(stream& s) noexcept
    : m_stream(s)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
stream_reader::~stream_reader()
{
    assert(!this->is_open());
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
const stream& stream_reader::get_input() const noexcept
{
    return this->m_stream;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
bool stream_reader::eof() const noexcept
{
    assert(this->m_stream.is_open());
    auto const result = this->m_stream.eof();
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void stream_reader::close() const
{
    assert(this->is_open());
    this->m_stream.close();
}

XR_NAMESPACE_END(xr, io)
//-----------------------------------------------------------------------------------------------------------