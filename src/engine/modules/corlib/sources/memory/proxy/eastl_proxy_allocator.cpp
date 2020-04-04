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

#include "corlib/memory/proxy/eastl_proxy_allocator.h"
#include "EASTL/memory.h"

//------------------------------------------------------------------------------
namespace xr::memory::proxy
{

//------------------------------------------------------------------------------
/**
*/
void* 
eastl_proxy_allocator::allocate(size_t n, int flags) const noexcept
{
    XR_UNREFERENCED_PARAMETER(flags);
    XR_DEBUG_ASSERTION(m_allocator != nullptr);

#if defined(_DEBUG)
    XR_DEBUG_ASSERTION(m_allocator_attached != false);
#endif // #if defined(_DEBUG)

    return m_allocator->malloc_impl(n
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("eastl_proxy_allocator")
        XR_DEBUG_PARAMETERS_DEFINITION);
}

//------------------------------------------------------------------------------
/**
*/
void* 
eastl_proxy_allocator::allocate(size_t n, size_t alignment, 
    size_t alignment_offset, int flags) const noexcept
{
    XR_UNREFERENCED_PARAMETER(alignment, alignment_offset, flags);
    XR_DEBUG_ASSERTION(m_allocator != nullptr);

#if defined(_DEBUG)
    XR_DEBUG_ASSERTION(m_allocator_attached != false);
#endif // #if defined(_DEBUG)

    return m_allocator->malloc_impl(n
        XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION("eastl_proxy_allocator")
        XR_DEBUG_PARAMETERS_DEFINITION);
}

//------------------------------------------------------------------------------
/**
*/
void 
eastl_proxy_allocator::deallocate(void* p, size_t n) const noexcept
{
    XR_UNREFERENCED_PARAMETER(n);
    XR_DEBUG_ASSERTION(m_allocator != nullptr);

#if defined(_DEBUG)
    XR_DEBUG_ASSERTION(m_allocator_attached != false);
#endif // #if defined(_DEBUG)

    m_allocator->free_impl(p XR_DEBUG_PARAMETERS_DEFINITION);
}

#if defined(_DEBUG)

//------------------------------------------------------------------------------
/**
*/
void 
eastl_proxy_allocator::set_name(eastl::string_view s) noexcept
{
    assert(eastl::size(m_allocator_name) <= s.size());
    eastl::copy(eastl::begin(s), eastl::end(s), m_allocator_name);
}

#endif // #if defined(_DEBUG)

} // namespace xr::memory::proxy
//------------------------------------------------------------------------------