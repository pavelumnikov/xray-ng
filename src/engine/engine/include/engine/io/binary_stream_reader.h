// This file is a part of xray-ng engine
//

#pragma once

#include "io/stream_reader.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, io)

//-----------------------------------------------------------------------------------------------------------
class binary_stream_reader final : public stream_reader
{
public:
    explicit binary_stream_reader(stream& s);
    ~binary_stream_reader();
}; // class binary_stream_reader

XR_NAMESPACE_END(xr, engine, io)
//-----------------------------------------------------------------------------------------------------------