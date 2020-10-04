// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/string_view.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t hash_crc32(char const* str, size_t s);

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t hash_crc32_continue(uint32_t original, char const* str, size_t s);

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint32_t hash_crc32(utils::string_view input)
{
    return hash_crc32(input.data(), input.size());
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint32_t hash_crc32_continue(uint32_t original, utils::string_view input)
{
    return hash_crc32_continue(original, input.data(), input.size());
}

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
