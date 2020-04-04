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

#include "corlib/platform.h"

//------------------------------------------------------------------------------
namespace xr::etl
{

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
/**
*/
template< uint32_t U, uint64_t ULL >
constexpr auto select_size_t_constant_v = select_size_t_constant<U, ULL>::value;

} // namespace xr::etl
//------------------------------------------------------------------------------