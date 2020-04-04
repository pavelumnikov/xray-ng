/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

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