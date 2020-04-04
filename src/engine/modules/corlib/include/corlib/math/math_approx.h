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

#include "corlib/math/math_const.h"
#include <cmath>

// Derived from batch testing
// TODO : Should be improved
#define IEEE_INT_RCP_CONST_NR0              0x7EF311C2
#define IEEE_INT_RCP_CONST_NR1              0x7EF311C3
#define IEEE_INT_RCP_CONST_NR2              0x7EF312AC

// Derived from batch testing
#define IEEE_INT_SQRT_CONST_NR0             0x1FBD1DF5

// Biases for global ranges
// 0-1 or 1-2 specific ranges might improve from different bias
// Derived from batch testing
// TODO : Should be improved
#define IEEE_INT_RCP_SQRT_CONST_NR0         0x5f3759df
#define IEEE_INT_RCP_SQRT_CONST_NR1         0x5F375A86
#define IEEE_INT_RCP_SQRT_CONST_NR2         0x5F375A86

//
// Normalized range [0,1] Constants
//
#define IEEE_INT_RCP_CONST_NR0_SNORM        0x7EEF370B
#define IEEE_INT_SQRT_CONST_NR0_SNORM       0x1FBD1DF5
#define IEEE_INT_RCP_SQRT_CONST_NR0_SNORM   0x5F341A43

//
// Distance [0,1000] based constants
//
#define IEEE_INT_RCP_CONST_NR0_SNORM        0x7EF3210C
#define IEEE_INT_SQRT_CONST_NR0_SNORM       0x1FBD22DF
#define IEEE_INT_RCP_SQRT_CONST_NR0_SNORM   0x5F33E79F

//------------------------------------------------------------------------------
namespace xr::math
{

//------------------------------------------------------------------------------
template<typename T>
constexpr int32_t
approx_as_int(T const value) noexcept
{
    return *reinterpret_cast<int*>(const_cast<T*>(&value));
}

//------------------------------------------------------------------------------
template<typename T>
constexpr float
approx_as_float(T const value) noexcept
{
    return *reinterpret_cast<float*>(const_cast<T*>(&value));
}

//------------------------------------------------------------------------------
// Approximate guess using integer float arithmetics based on IEEE 
// floating point standard
constexpr float
rcp_sqrt_ieee_int_approx(float const in, int const in_rcp_sqrt_const) noexcept
{
    auto const x = approx_as_int(in);
    auto const x1 = (in_rcp_sqrt_const - (x >> 1));
    return approx_as_float(x1);
}

//------------------------------------------------------------------------------
constexpr float 
sqrt_ieee_int_approx(float const in, int const in_sqrt_const) noexcept
{
    auto const x = approx_as_int(in);
    auto const x1 = in_sqrt_const + (x >> 1);
    return approx_as_float(x1);
}

//------------------------------------------------------------------------------
constexpr float
rcp_ieee_int_approx(float const in, int const in_rcp_const)
{
    auto const x = approx_as_int(in);
    auto const x1 = (in_rcp_const - x);
    return approx_as_float(x1);
}

//------------------------------------------------------------------------------
constexpr float
rcp_sqrt_newton_raphson(float const in, float const in_rcp) noexcept
{
    return in_rcp * (-in * (in_rcp * in_rcp) + 1.5f);
}

//------------------------------------------------------------------------------
constexpr float
rcp_newton_raphson(float const in, float const in_rcp) noexcept
{
    return in_rcp * (-in_rcp * in + 2.0f);
}

//------------------------------------------------------------------------------
// Using 1 Newton Raphson iterations
// Relative error : ~0.2% over full
// Precise format : ~half float
// 6 ALU
//
constexpr float
fast_rcp_sqrt_nr1(float const in) noexcept
{
    auto const half_float = 0.5f * in;
    auto rcp = rcp_sqrt_ieee_int_approx(in, IEEE_INT_RCP_SQRT_CONST_NR1);
    rcp = rcp_sqrt_newton_raphson(half_float, rcp);

    return rcp;
}

//------------------------------------------------------------------------------
// Using 2 Newton Raphson iterations
// Relative error : ~4.6e-004%  over full
// Precise format : ~full float
// 9 ALU
//
constexpr float
fast_rcp_sqrt_nr2(float const in) noexcept
{
    auto const half_float = 0.5f * in;
    auto rcp = rcp_sqrt_ieee_int_approx(in, IEEE_INT_RCP_SQRT_CONST_NR2);
    rcp = rcp_sqrt_newton_raphson(half_float, rcp);
    rcp = rcp_sqrt_newton_raphson(half_float, rcp);

    return rcp;
}

//------------------------------------------------------------------------------
// Using 0 Newton Raphson iterations
// Relative error : < 0.7% over full
// Precise format : ~small float
// 1 ALU
//
constexpr float
fast_sqrt_nr0(float const in) noexcept
{
    float  rcp = sqrt_ieee_int_approx(in, IEEE_INT_SQRT_CONST_NR0);
    return rcp;
}

//------------------------------------------------------------------------------
// Use inverse Rcp Sqrt
// Using 1 Newton Raphson iterations
// Relative error : ~0.2% over full
// Precise format : ~half float
// 6 ALU
//
constexpr float
fast_sqrt_nr1(float const in) noexcept
{
    // Inverse Rcp Sqrt
    return in * fast_rcp_sqrt_nr1(in);
}

//------------------------------------------------------------------------------
// Use inverse Rcp Sqrt
// Using 2 Newton Raphson iterations
// Relative error : ~4.6e-004%  over full
// Precise format : ~full float
// 9 ALU
//
constexpr float
fast_sqrt_nr2(float const in) noexcept
{
    // Inverse Rcp Sqrt
    return in * fast_rcp_sqrt_nr2(in);
}

//------------------------------------------------------------------------------
// Using 0 Newton Raphson iterations
// Relative error : < 0.4% over full
// Precise format : ~small float
// 1 ALU
//
constexpr float
fast_rcp_nr0(float const in) noexcept
{
    auto const rcp = rcp_ieee_int_approx(in, IEEE_INT_RCP_CONST_NR0);
    return rcp;
}

//------------------------------------------------------------------------------
// Using 1 Newton Raphson iterations
// Relative error : < 0.02% over full
// Precise format : ~half float
// 3 ALU
//
constexpr float
fast_rcp_nr1(float in) noexcept
{
    auto rcp = rcp_ieee_int_approx(in, IEEE_INT_RCP_CONST_NR1);
    rcp = rcp_newton_raphson(in, rcp);
    return rcp;
}

//------------------------------------------------------------------------------
// Using 2 Newton Raphson iterations
// Relative error : < 5.0e-005%  over full
// Precise format : ~full float
// 5 ALU
//
constexpr float
fast_rcp_nr2(float in) noexcept
{
    auto rcp = rcp_ieee_int_approx(in, IEEE_INT_RCP_CONST_NR2);
    rcp = rcp_newton_raphson(in, rcp);
    rcp = rcp_newton_raphson(in, rcp);

    return rcp;
}

//------------------------------------------------------------------------------
// 4th order polynomial approximation
// 4 VGRP, 16 ALU Full Rate
// 7 * 10^-5 radians precision
// Reference : Handbook of Mathematical Functions 
// (chapter : Elementary Transcendental Functions), M. Abramowitz and I.A. Stegun, Ed.
inline float
acos_fast4(float in)
{
    auto const x1 = std::abs(in);
    auto const x2 = x1 * x1;
    auto const x3 = x2 * x1;
    auto s = 0.0f;

    s = -0.2121144f * x1 + 1.5707288f;
    s = 0.0742610f * x2 + s;
    s = -0.0187293f * x3 + s;
    s = sqrt(1.0f - x1) * s;

    // acos function mirroring
    // check per platform if compiles to a selector - no branch neeeded
    return (in >= 0.0f) ? s : pi - s;
}

//------------------------------------------------------------------------------
// 4th order polynomial approximation
// 4 VGRP, 16 ALU Full Rate
// 7 * 10^-5 radians precision 
inline float
asin_fast4(float const in)
{
    auto const x = in;

    // asin is offset of acos
    return pi_div_2 - acos_fast4(x);
}

//------------------------------------------------------------------------------
// 4th order hyperbolical approximation
// 4 VGRP, 12 ALU Full Rate
// 7 * 10^-5 radians precision 
// Reference : Efficient approximations for the arctangent function, 
// Rajan, S. Sichun Wang Inkol, R. Joyal, A., May 2006
inline float
atan_fast4(float const in)
{
    auto const x = in;
    return x * (-0.1784f * std::abs(x) - 0.0663f * x * x + 1.0301f);
}

} // namespace xr::math
//------------------------------------------------------------------------------