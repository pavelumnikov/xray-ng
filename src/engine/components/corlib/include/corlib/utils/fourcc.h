// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE uint32_t fourcc_from_str(char const p[5])
{
    return static_cast<uint32_t>(static_cast<uint32_t>(p[0]) << 24) |
        (static_cast<uint32_t>(p[1]) << 16) | (static_cast<uint32_t>(p[2]) << 8) | static_cast<uint32_t>(p[3]);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE uint32_t fourcc_from_vals(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return static_cast<uint32_t>(static_cast<uint32_t>(d) << 24) |
        (static_cast<uint32_t>(c) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
}

//-----------------------------------------------------------------------------------------------------------
template <uint8_t a, uint8_t b, uint8_t c, uint8_t d>
struct fourcc_ct
{
    static XR_CONSTEXPR_CPP14_OR_CONST uint32_t value = fourcc_from_vals(a, b, c, d);
}; // struct fourcc<a,b,c,d>

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------