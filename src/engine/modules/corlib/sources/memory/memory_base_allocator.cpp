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

#include "corlib/memory/memory_allocator_base.h"
#include "EASTL/internal/move_help.h"

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
/**
*/
void
base_allocator::on_malloc(pvoid buffer, size_t buffer_size, size_t previous_size,
    eastl::string_view description XR_DEBUG_PARAMETERS_DECLARATION) const
{
    if(base_profiler_event_listener* event_listener = m_event_listener)
    {
        event_listener->on_malloc_done(buffer, buffer_size, previous_size,
            description XR_DEBUG_PARAMETERS);
    }
}

//------------------------------------------------------------------------------
/**
*/
void
base_allocator::on_free(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) const
{
    if(base_profiler_event_listener* event_listener = m_event_listener)
    {
        event_listener->on_free_done(buffer XR_DEBUG_PARAMETERS);
    }
}

//------------------------------------------------------------------------------
/**
*/
pvoid base_allocator::malloc_impl(size_t size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    pvoid ptr = call_malloc(size XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    on_malloc(ptr, size, 0 XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
pvoid base_allocator::realloc_impl(pvoid pointer, size_t new_size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    pvoid ptr = call_realloc(pointer, new_size XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    on_malloc(ptr, new_size, 0 XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
void base_allocator::free_impl(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    on_free(pointer XR_DEBUG_PARAMETERS);
    call_free(pointer XR_DEBUG_PARAMETERS);
}

} // namespace xr::memory
//------------------------------------------------------------------------------