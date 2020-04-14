// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::etl
{

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr uint32_t fourcc_from_str(char const p[5])
{
    return static_cast<uint32_t>(static_cast<uint32_t>(p[0]) << 24) |
        (static_cast<uint32_t>(p[1]) << 16) | (static_cast<uint32_t>(p[2]) << 8) | static_cast<uint32_t>(p[3]);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
constexpr uint32_t fourcc_from_vals(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return static_cast<uint32_t>(static_cast<uint32_t>(d) << 24) |
        (static_cast<uint32_t>(c) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
}

//-----------------------------------------------------------------------------------------------------------
template <uint8_t a, uint8_t b, uint8_t c, uint8_t d>
struct fourcc_ct
{
    static constexpr uint32_t value = fourcc_from_vals(a, b, c, d);
}; // struct fourcc<a,b,c,d>

//-----------------------------------------------------------------------------------------------------------
template <uint8_t a, uint8_t b, uint8_t c, uint8_t d>
constexpr auto fourcc_ct_v = fourcc_ct<a, b, c, d>::value;

} // namespace xr::etl
//-----------------------------------------------------------------------------------------------------------