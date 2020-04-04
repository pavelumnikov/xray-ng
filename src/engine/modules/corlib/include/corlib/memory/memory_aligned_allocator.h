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

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
template<size_t kAlignment>
class aligned_allocator final : public base_allocator
{
    using super = base_allocator;

public:
    aligned_allocator(memory::base_allocator& alloc);
    constexpr size_t alignment() const;

    virtual bool can_allocate_block(size_t const size) const noexcept override;
    virtual size_t allocated_size() const noexcept override;
    virtual size_t total_size() const noexcept override;

protected:
    pvoid call_malloc(size_t size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    pvoid call_realloc(pvoid pointer, size_t new_size
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void call_free(pvoid pointer
        XR_DEBUG_PARAMETERS_DECLARATION) override;

private:
    static size_t calculate_size(size_t size);
    memory::base_allocator& m_allocator;
}; // class crt_allocator

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline aligned_allocator<kAlignment>::aligned_allocator(memory::base_allocator& alloc)
    : m_allocator { alloc }
{}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline bool
aligned_allocator<kAlignment>::can_allocate_block(size_t const size) const noexcept
{
    return m_allocator.can_allocate_block(size + kAlignment);
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline size_t
aligned_allocator<kAlignment>::allocated_size() const noexcept
{
    return m_allocator.allocated_size();
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline size_t
aligned_allocator<kAlignment>::total_size() const noexcept
{
    return m_allocator.total_size();
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
constexpr size_t 
aligned_allocator<kAlignment>::alignment() const
{
    return kAlignment;
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline pvoid
aligned_allocator<kAlignment>::call_malloc(size_t size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    // Allocate necessary memory area client request - size parameter -
    // plus area to store the address of the memory returned by standard
    // malloc().

    // accomodating atleast one aligned memory area by adding align-1
    // size of (void *) is added since we intent to keep the below address
    // in p. This will be retrieved later during free

    pvoid p = m_allocator.malloc_impl(calculate_size(size)
        XR_DEBUG_PARAMETERS_DESCRIPTION
        XR_DEBUG_PARAMETERS);

    if(p != nullptr)
    {
        // Address of the aligned memory according to the align parameter
        pvoid ptr = (pvoid)(((size_t)p + sizeof(pvoid) + kAlignment - 1) & ~(kAlignment - 1));
        // store the address of the malloc() above the beginning of our total memory area.
        *((pvoid*)((size_t)ptr - sizeof(pvoid))) = p;
        // Return the address of aligned memory
        return ptr;
    }

    return nullptr;
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline pvoid
aligned_allocator<kAlignment>::call_realloc(pvoid pointer, size_t new_size
    XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    return nullptr;
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline void aligned_allocator<kAlignment>::call_free(pvoid pointer
    XR_DEBUG_PARAMETERS_DECLARATION)
{
    pvoid ptr = *((pvoid*)((size_t)pointer - sizeof(pvoid)));
    m_allocator.free_impl(ptr XR_DEBUG_PARAMETERS);
}

//------------------------------------------------------------------------------
/**
 */
template<size_t kAlignment>
inline size_t aligned_allocator<kAlignment>::calculate_size(size_t size)
{
    return size + (kAlignment - 1) + sizeof(pvoid);
}

} // namespace xr::memory
//------------------------------------------------------------------------------