// This file is a part of xray-ng engine
//

#include "engine/io/stream_writer.h"
#include "EASTL/internal/copy_help.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

stream_writer::stream_writer(stream& s)
    : m_stream(s)
{}

stream_writer::~stream_writer()
{
    assert(!this->is_open());
}

const stream& stream_writer::get_input() const
{
    return this->m_stream;
}

bool stream_writer::eof() const
{
    assert(this->m_stream.is_open());
    auto const result = this->m_stream.eof();
    return result;
}

void stream_writer::close() const
{
    assert(this->m_stream.is_open());
    this->m_stream.close();
}

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------
