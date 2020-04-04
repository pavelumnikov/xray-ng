/*

  Copyright (c) 2020, Pavel Umnikov
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
namespace xr::memory
{

//------------------------------------------------------------------------------
class XR_NON_VIRTUAL base_profiler_event_listener
{
public:
    virtual ~base_profiler_event_listener()
    {};

    // Called from memory allocator on allocation/reallocation
    virtual void on_malloc_done(pvoid buffer, size_t buffer_size, size_t previous_size,
        eastl::string_view description XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    // Called from memory allocator on free
    virtual void on_free_done(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) = 0;
}; // class base_profiler_event_listener

} // namespace xr::memory