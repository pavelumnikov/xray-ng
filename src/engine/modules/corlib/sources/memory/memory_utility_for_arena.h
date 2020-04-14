// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::memory
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline uintptr_t internal_adjust_alignment(uintptr_t alignment)
{
    return (alignment > sizeof(pvoid) ? alignment : sizeof(pvoid)) - 1;
}

//-----------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------------------------------
