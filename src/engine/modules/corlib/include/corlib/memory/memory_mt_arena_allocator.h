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

namespace xr::memory
{

class mt_arena_allocator final : public base_allocator 
{
public:
    mt_arena_allocator() = default;

    void initialize(size_t size, size_t initial);

    virtual bool can_allocate_block(size_t const size) const noexcept override;
    virtual size_t total_size() const noexcept override;
    virtual size_t allocated_size() const noexcept override;

private:
    pvoid call_malloc(size_t size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    pvoid call_realloc(pvoid pointer, size_t new_size 
        XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void call_free(pvoid pointer 
        XR_DEBUG_PARAMETERS_DECLARATION) override;

    void finalize();

    pvoid m_arena;
}; // class mt_arena_allocator

} // namespace xr::memory
