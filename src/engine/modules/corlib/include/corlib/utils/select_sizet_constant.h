// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/platform.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::utils
{

//-----------------------------------------------------------------------------------------------------------
// A template to select either 32-bit or 64-bit constant as compile time, 
// depending on machine word size. Explicit cast is needed to avoid compiler 
// warnings about possible truncation. The value of the right size, which is 
// selected by ?:, is anyway not truncated or promoted.
//
template< uint32_t U, uint64_t ULL >
struct select_size_t_constant final
{
	static constexpr size_t value = 
        static_cast<size_t>((sizeof(size_t) == sizeof(uint32_t)) ? U : ULL);
};

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< uint32_t U, uint64_t ULL >
constexpr auto select_size_t_constant_v = select_size_t_constant<U, ULL>::value;

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
