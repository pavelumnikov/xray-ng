// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, memory)

//-----------------------------------------------------------------------------------------------------------
/**
*/
void copy(void* destination, size_t const destination_size, void const* source, size_t const source_size);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void copy_align_16(void* destination, size_t const destination_size, void const* source, size_t const source_size);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void zero(void* destination, size_t const size_in_bytes);

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T, size_t Count>
void zero(T(&destination)[Count])
{
    zero(destination, Count * sizeof(T));
}

XR_NAMESPACE_END(xr, memory)
//-----------------------------------------------------------------------------------------------------------
