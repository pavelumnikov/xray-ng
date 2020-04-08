// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"

#define XR_MAX_CACHE_LINE_PAD char XR_STRING_CONCAT(m_cache_line_pad_$, __LINE__) [XR_MAX_CACHE_LINE_SIZE]

//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
//! Basic padding with template selection
template< typename Type, size_t S, size_t CacheLineSize = XR_MAX_CACHE_LINE_SIZE >
struct padded_base : Type
{
    int8_t pad[CacheLineSize - sizeof(Type) % CacheLineSize];
};

//------------------------------------------------------------------------------
//! Basic padding with default "0" selection parameter
template< typename Type >
struct padded_base< Type, 0 > : Type
{};


//------------------------------------------------------------------------------
//! Pads type T to fill out to a multiple of cache line size.
template< typename Type >
using padded = padded_base< Type, sizeof(Type) >;

//------------------------------------------------------------------------------
//! Pads type T to fill out to a multiple of cache line size(with custom sizing).
template< typename Type >
using padded_with_cache_line_size = padded_base< Type, sizeof(Type), Type::cache_line_size >;

} // namespace xr::etl
//------------------------------------------------------------------------------