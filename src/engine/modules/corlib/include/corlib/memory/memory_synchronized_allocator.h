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

#include "corlib/memory/memory_allocator_base.h"
#include "corlib/threading/scoped_lock.h"

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
template<class Allocator, class Mutex>
class memory_synchronized_allocator : public Allocator
{
public:
    using allocator = Allocator;

    memory_synchronized_allocator() noexcept = default;

protected:
    virtual pvoid call_malloc(size_t size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual pvoid call_realloc(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    virtual void call_free(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION) override;

private:
    threading::mutex m_mutex;
}; // class memory_synchronized_allocator

//------------------------------------------------------------------------------
/**
 */
template<class Allocator, class Mutex>
inline pvoid
memory_synchronized_allocator<Allocator, Mutex>::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    threading::scoped_lock lock { m_mutex };
    pvoid out_result = allocator::malloc_impl(size
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);

    return out_result;
}

//------------------------------------------------------------------------------
/**
 */
template<class Allocator, class Mutex>
inline pvoid
memory_synchronized_allocator<Allocator, Mutex>::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    threading::scoped_lock lock { m_mutex };
    pvoid out_result = allocator::realloc_impl(pointer, new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION XR_DEBUG_PARAMETERS);

    return out_result;
}

//------------------------------------------------------------------------------
/**
 */
template<class Allocator, class Mutex>
inline void
memory_synchronized_allocator<Allocator, Mutex>::call_free(pvoid pointer
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    threading::scoped_lock lock { m_mutex };
    super::free_impl(pointer XR_DEBUG_PARAMETERS);
}

} // namespace xr::memory
//------------------------------------------------------------------------------