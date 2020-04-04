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
#include "EASTL/string_view.h"

namespace xr::memory::proxy
{

///////////////////////////////////////////////////////////////////////////////
// eastl_proxy_allocator
//
// Implements an EASTL allocator that uses malloc/free as opposed to 
// new/delete or PPMalloc Malloc/Free. 
//
// Example usage:
//      eastl::vector<int, eastl_proxy_allocator> int_vector;
// 
class eastl_proxy_allocator final
{
public:
    static constexpr char const default_name[] = "eastl_proxy_allocator";

    explicit eastl_proxy_allocator(char const* s = default_name) noexcept
        : m_allocator { nullptr }
#if defined(_DEBUG)
        , m_allocator_attached { false }
        , m_allocator_name {}
#endif
    {}

    explicit eastl_proxy_allocator(base_allocator& alloc, char const* s = default_name) noexcept
        : m_allocator { &alloc }
#if defined(_DEBUG)
        , m_allocator_attached { true }
        , m_allocator_name {}
#endif
    {}

    eastl_proxy_allocator(const eastl_proxy_allocator& rhs, char const* s) noexcept
        : m_allocator(rhs.m_allocator)
#if defined(_DEBUG)
        , m_allocator_attached(true)
        , m_allocator_name {}
#endif
    {}

    XR_DECLARE_DEFAULT_DESTRUCTOR(eastl_proxy_allocator);

    eastl_proxy_allocator(eastl_proxy_allocator const&) noexcept = default;
    eastl_proxy_allocator(eastl_proxy_allocator&&) noexcept = default;

    eastl_proxy_allocator& operator=(const eastl_proxy_allocator&) noexcept = default;
    eastl_proxy_allocator& operator=(eastl_proxy_allocator&&) noexcept = default;

    bool operator==(const eastl_proxy_allocator&) const noexcept
    {
        return true;
    }

    bool operator!=(const eastl_proxy_allocator&) const noexcept
    {
        return false;
    }

    void* allocate(size_t n, int flags = 0) const noexcept;
    void* allocate(size_t n, size_t alignment, size_t alignment_offset, int flags = 0) const noexcept;
    void deallocate(void* p, size_t n) const noexcept;

    const char* get_name() const noexcept
    {
#if defined(_DEBUG)
        return this->m_allocator_name;
#else
        return default_name;
#endif
    }

    void set_name(eastl::string_view s) noexcept;

    void set_base_allocator(base_allocator& alloc) noexcept;

private:
    base_allocator* m_allocator;

#if defined(_DEBUG)
    bool m_allocator_attached;
    char m_allocator_name[64];
#endif // defined(DEBUG)
}; // class eastl_proxy_allocator

#if !defined(_DEBUG)
//------------------------------------------------------------------------------
/**
 */
void 
eastl_proxy_allocator::set_name(eastl::string_view s) const noexcept
{}

#endif

//------------------------------------------------------------------------------
/**
 */
inline void 
eastl_proxy_allocator::set_base_allocator(base_allocator& alloc) noexcept
{
    XR_DEBUG_ASSERTION(!this->m_allocator_attached);

    this->m_allocator = &alloc;
    this->m_allocator_attached = true;
}

} // namespace xr::memory::proxy
//------------------------------------------------------------------------------