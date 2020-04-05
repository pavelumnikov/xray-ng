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

#include "corlib/memory/allocator_helper.h"
#include "corlib/etl/algorithms/aligning.h"
#include "corlib/etl/type_traits.h"
#include "corlib/memory/memory_debug_parameters.h"
#include "corlib/memory/memory_functions.h"
#include "corlib/memory/profiler_event_listener.h"

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
class base_allocator
{
public:
    virtual ~base_allocator() = default;

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(base_allocator);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(base_allocator);

    pvoid malloc_impl(size_t size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION);

    pvoid realloc_impl(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION);

    void free_impl(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION);

    void set_event_listener(base_profiler_event_listener* event_listener);

    virtual bool can_allocate_block(size_t const size) const noexcept = 0;
    virtual size_t total_size() const noexcept = 0;
    virtual size_t allocated_size() const noexcept = 0;

protected:
    constexpr base_allocator() noexcept = default;

    virtual pvoid call_malloc(size_t size_t 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    virtual void call_free(pvoid pointer 
        XR_DEBUG_PARAMETERS_DECLARATION) = 0;

    void on_malloc(pvoid buffer, size_t buffer_size, size_t previous_size, 
        eastl::string_view description XR_DEBUG_PARAMETERS_DECLARATION) const;

    void on_free(pvoid& buffer XR_DEBUG_PARAMETERS_DECLARATION) const;

private:
    base_profiler_event_listener* m_event_listener { nullptr };
}; // class base_allocator

//------------------------------------------------------------------------------
/**
 */
inline void
base_allocator::set_event_listener(base_profiler_event_listener* event_listener)
{
    m_event_listener = event_listener;
}

} // namespace xr::memory
//-----------------------------------------------------------------------------