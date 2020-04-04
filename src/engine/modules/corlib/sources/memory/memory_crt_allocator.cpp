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

#include "corlib/memory/memory_crt_allocator.h"
#include <cassert>

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
/**
*/
pvoid crt_allocator::call_malloc(size_t size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;

#ifdef _DEBUG
    XR_UNREFERENCED_PARAMETER(description);
#endif // #ifdef DEBUG

    assert(this->m_malloc_ptr);

    auto const real_size = size;
    pvoid const result = (*this->m_malloc_ptr)(real_size);

    return result;
}

//------------------------------------------------------------------------------
/**
*/
pvoid crt_allocator::call_realloc(pvoid pointer, size_t new_size 
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION XR_DEBUG_PARAMETERS_DECLARATION)
{
#ifdef _DEBUG
    XR_UNREFERENCED_PARAMETER(description);
#endif // #ifdef DEBUG

    if(!new_size)
    {
        this->free_impl(pointer XR_DEBUG_PARAMETERS);
        return (0);
    }

    assert(this->m_realloc_ptr);
    auto const real_size = new_size;
    pvoid const result = (*this->m_realloc_ptr)(pointer, real_size);

    return result;
}

//------------------------------------------------------------------------------
/**
*/
void crt_allocator::call_free(pvoid pointer XR_DEBUG_PARAMETERS_DECLARATION)
{
    XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD;

    if(!pointer)
        return;

    assert(this->m_free_ptr);
    (*this->m_free_ptr)(pointer);
}

} // namespace xr::memory
//------------------------------------------------------------------------------