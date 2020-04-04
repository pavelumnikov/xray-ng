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

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
/**
*/
inline uintptr_t internal_adjust_alignment(uintptr_t alignment)
{
    return (alignment > sizeof(pvoid) ? alignment : sizeof(pvoid)) - 1;
}

//------------------------------------------------------------------------------
/**
*/
inline pvoid
align_pointer(pvoid p, uintptr_t align, uintptr_t offset) noexcept
{
    auto const alignment = internal_adjust_alignment(align);

    // gap = number of bytes needed to round up offset to align with sizeof(pvoid)
    auto const gap = (0 - offset) & (sizeof(pvoid) - 1);
    auto const ptr = reinterpret_cast<uintptr_t>(p);

    uintptr_t retptr =
        ((ptr + sizeof(pvoid) + gap + alignment + offset)&~alignment) - offset;

    (reinterpret_cast<uintptr_t*>(retptr - gap))[-1] = ptr;

    return reinterpret_cast<pvoid>(retptr);
}

//------------------------------------------------------------------------------
/**
*/
inline void* 
unalign_pointer(pvoid p) noexcept
{
    auto ptr = reinterpret_cast<uintptr_t>(p);
    // ptr points to the pointer to starting of the memory block
    ptr = (ptr & ~(sizeof(pvoid) - 1)) - sizeof(pvoid);
    // ptr is the pointer to the start of memory block
    ptr = *(reinterpret_cast<uintptr_t*>(ptr));

    return reinterpret_cast<pvoid>(ptr);
}

} // namespace xr::memory
//------------------------------------------------------------------------------