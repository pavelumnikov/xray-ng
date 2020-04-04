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

#include "EASTL/numeric_limits.h"

//------------------------------------------------------------------------------
namespace xr::math
{

auto constexpr max_uint32 = eastl::numeric_limits<uint32_t>::max();
auto constexpr min_int32 = eastl::numeric_limits<int32_t>::min();
auto constexpr max_int32 = eastl::numeric_limits<int32_t>::max();
auto const min_float = eastl::numeric_limits<float>::min();
auto const max_float = eastl::numeric_limits<float>::max();
auto constexpr min_pos_float = 1.175494351e-38F;

auto constexpr pi = 3.141592654f;
auto constexpr two_pi = 6.283185307f;
auto constexpr ri_recip = 0.318309886f;
auto constexpr two_pi_recip = 0.159154943f;
auto constexpr pi_div_2 = 1.570796327f;
auto constexpr pi_div_4 = 0.785398163f;

auto constexpr eps = 0.000001f;
// Very small epsilon for checking against 0.0f
auto constexpr zero_eps = (32.0f * min_pos_float);
auto const nan = eastl::numeric_limits<float>::quiet_NaN();

} // namespace xr::math
//------------------------------------------------------------------------------