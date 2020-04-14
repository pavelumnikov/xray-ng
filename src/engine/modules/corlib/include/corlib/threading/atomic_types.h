// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::threading
{

using atomic_bool = volatile bool;
using atomic_int8 = volatile int8_t;
using atomic_uint8 = volatile uint8_t;
using atomic_int16 = volatile int16_t;
using atomic_uint16 = volatile uint16_t;
using atomic_int32 = volatile int32_t;
using atomic_uint32 = volatile uint32_t;
using atomic_int64 = volatile int64_t;
using atomic_uint64 = volatile uint64_t;
using atomic_size_t = volatile size_t;

} // namespace xr::threading
//-----------------------------------------------------------------------------------------------------------