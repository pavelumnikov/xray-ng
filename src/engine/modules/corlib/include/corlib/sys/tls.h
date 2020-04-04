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

#include "corlib/types.h"
#include "EASTL/string_view.h"

//------------------------------------------------------------------------------
namespace xr::sys
{

//------------------------------------------------------------------------------
using tls_handle = uint32_t;

uintptr_t constexpr invalid_thread_local_storage =
    (eastl::numeric_limits<tls_handle>::max)();

//------------------------------------------------------------------------------
/**
*/
tls_handle create_thread_local();

//------------------------------------------------------------------------------
/**
*/
bool set_tls_data(tls_handle const tls, void* data);

//------------------------------------------------------------------------------
/**
*/
void* get_tls_data(tls_handle const tls);

//------------------------------------------------------------------------------
/**
*/
template<typename T>
T* get_tls_typed_data(tls_handle const tls)
{
    return reinterpret_cast<T*>(sys::get_tls_data(tls));
}

//------------------------------------------------------------------------------
/**
*/
void destroy_thread_local(tls_handle const tls);

} // namespace xr::sys
//------------------------------------------------------------------------------