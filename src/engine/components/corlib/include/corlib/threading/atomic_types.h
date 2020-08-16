// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, threading)

typedef volatile bool atomic_bool;
typedef volatile int8_t atomic_int8;
typedef volatile uint8_t atomic_uint8;
typedef volatile int16_t atomic_int16;
typedef volatile uint16_t atomic_uint16;
typedef volatile int32_t atomic_int32;
typedef volatile uint32_t atomic_uint32;
typedef volatile int64_t atomic_int64;
typedef volatile uint64_t atomic_uint64;
typedef volatile size_t atomic_size_t;

XR_NAMESPACE_END(xr, threading)
//-----------------------------------------------------------------------------------------------------------