// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/constants.h"
#include "corlib/utils/type_inversions.h"
#include <cmath>

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint32_t
float_sign_bit_set(float x) XR_NOEXCEPT
{
    return (static_cast<uint32_t>(x) >> 31);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
float_is_nan(float x) XR_NOEXCEPT
{
    return (((static_cast<uint32_t>(x)) & 0x7f800000) == 0x7f800000);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
float_is_inf(float x) XR_NOEXCEPT
{
    return (((static_cast<uint32_t>(x)) & 0x7fffffff) == 0x7f800000);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
float_is_ind(float x) XR_NOEXCEPT
{
    return ((static_cast<uint32_t>(x)) == 0xffc00000);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool
float_is_denormal(float x) XR_NOEXCEPT
{
    return (((static_cast<uint32_t>(x)) & 0x7f800000) == 0x00000000 &&
        ((static_cast<uint32_t>(x)) & 0x007fffff) != 0x00000000);
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE int32_t max_index(T x, T y) XR_NOEXCEPT
{
    return  (x > y) ? 0 : 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE int32_t min_index(T x, T y) XR_NOEXCEPT
{
    return (x < y) ? 0 : 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE T max3(T x, T y, T z) XR_NOEXCEPT
{
    return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE T min3(T x, T y, T z) XR_NOEXCEPT
{
    return (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE int32_t max3_index(T x, T y, T z) XR_NOEXCEPT
{
    return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE int32_t min3_index(T x, T y, T z) XR_NOEXCEPT
{
    return (x < y) ? ((x < z) ? 0 : 2) : ((y < z) ? 1 : 2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE T sign(T f)
{
    return (f > 0) ? 1 : ((f < 0) ? -1 : 0);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T>
XR_CONSTEXPR_CPP14_OR_INLINE T square(T x)
{
    return x * x;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<class T> XR_CONSTEXPR_CPP14_OR_INLINE T cube(T x)
{
    return x * x * x;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool
negative(float const& f)
{
    return (static_cast<uint32_t>(f) & float_mask_sign_bit);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool
positive(float const& f)
{
    return (static_cast<uint32_t>(f) & float_mask_sign_bit) == 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
approx_as_int(T const value) XR_NOEXCEPT
{
    return static_cast<int32_t>(value);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE float
approx_as_float(T const value) XR_NOEXCEPT
{
    return static_cast<float>(value);
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Approximate guess using integer float arithmetics based on IEEE 
 * floating point standard
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
rcp_sqrt_ieee_int_approx(float const in, int32_t const in_rcp_sqrt_const) XR_NOEXCEPT
{
    auto const x = approx_as_int(in);
    auto const x1 = (in_rcp_sqrt_const - (x >> 1));
    return approx_as_float(x1);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
sqrt_ieee_int_approx(float const in, int32_t const in_sqrt_const) XR_NOEXCEPT
{
    auto const x = approx_as_int(in);
    auto const x1 = in_sqrt_const + (x >> 1);
    return approx_as_float(x1);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
rcp_ieee_int_approx(float const in, int32_t const in_rcp_const)
{
    auto const x = approx_as_int(in);
    auto const x1 = (in_rcp_const - x);
    return approx_as_float(x1);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
rcp_sqrt_newton_raphson(float const in, float const in_rcp) XR_NOEXCEPT
{
    return in_rcp * (-in * (in_rcp * in_rcp) + 1.5f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
rcp_newton_raphson(float const in, float const in_rcp) XR_NOEXCEPT
{
    return in_rcp * (-in_rcp * in + 2.0f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Using 1 Newton Raphson iterations
 * Relative error : ~0.2% over full
 * Precise format : ~half float
 * 6 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_rcp_sqrt_nr1(float const in) XR_NOEXCEPT
{
    auto const half_float = 0.5f * in;
    auto rcp = rcp_sqrt_ieee_int_approx(in, ieee_int_rcp_sqrt_const_nr1);
    rcp = rcp_sqrt_newton_raphson(half_float, rcp);

    return rcp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Using 2 Newton Raphson iterations
 * Relative error : ~4.6e-004%  over full
 * Precise format : ~full float
 * 9 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_rcp_sqrt_nr2(float const in) XR_NOEXCEPT
{
    auto const half_float = 0.5f * in;
    auto rcp = rcp_sqrt_ieee_int_approx(in, ieee_int_rcp_sqrt_const_nr2);
    rcp = rcp_sqrt_newton_raphson(half_float, rcp);
    rcp = rcp_sqrt_newton_raphson(half_float, rcp);

    return rcp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Using 0 Newton Raphson iterations
 * Relative error : < 0.7% over full
 * Precise format : ~small float
 * 1 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_sqrt_nr0(float const in) XR_NOEXCEPT
{
    float  rcp = sqrt_ieee_int_approx(in, ieee_int_sqrt_const_nr0);
    return rcp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Use inverse Rcp Sqrt
 * Using 1 Newton Raphson iterations
 * Relative error : ~0.2% over full
 * Precise format : ~half float
 * 6 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_sqrt_nr1(float const in) XR_NOEXCEPT
{
    // Inverse Rcp Sqrt
    return in * fast_rcp_sqrt_nr1(in);
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Use inverse Rcp Sqrt
 * Using 2 Newton Raphson iterations
 * Relative error : ~4.6e-004%  over full
 * Precise format : ~full float
 * 9 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_sqrt_nr2(float const in) XR_NOEXCEPT
{
    // Inverse Rcp Sqrt
    return in * fast_rcp_sqrt_nr2(in);
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Using 0 Newton Raphson iterations
 * Relative error : < 0.4% over full
 * Precise format : ~small float
 * 1 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_rcp_nr0(float const in) XR_NOEXCEPT
{
    auto const rcp = rcp_ieee_int_approx(in, ieee_int_rcp_const_nr0);
    return rcp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Using 1 Newton Raphson iterations
 * Relative error : < 0.02% over full
 * Precise format : ~half float
 * 3 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_rcp_nr1(float in) XR_NOEXCEPT
{
    auto rcp = rcp_ieee_int_approx(in, ieee_int_rcp_const_nr1);
    rcp = rcp_newton_raphson(in, rcp);
    return rcp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 * Using 2 Newton Raphson iterations
 * Relative error : < 5.0e-005%  over full
 * Precise format : ~full float
 * 5 ALU
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
fast_rcp_nr2(float in) XR_NOEXCEPT
{
    auto rcp = rcp_ieee_int_approx(in, ieee_int_rcp_const_nr2);
    rcp = rcp_newton_raphson(in, rcp);
    rcp = rcp_newton_raphson(in, rcp);

    return rcp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
rsqrt(float x) XR_NOEXCEPT
{
    return fast_rcp_sqrt_nr2(x);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
inv_sqrt(float x)
{
    return 1 / fast_rcp_sqrt_nr2(x);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float
sqrt(float x)
{
    return x * inv_sqrt(x);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
sine(float a)
{
    return sinf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
cosine(float a)
{
    return ::cosf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void
sin_cos(float a, float& s, float& c)
{
    s = ::sinf(a);
    c = ::cosf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
tan(float a)
{
    return ::tanf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline double
tan64(float const a)
{
    return ::tan(static_cast<double>(a));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
asin(float const a)
{
    if(a <= -1.0f)
        return -pi_div_2;

    if(a >= 1.0f)
        return pi_div_2;

    return ::asinf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
acos(float a)
{
    if(a <= -1.0f)
        return pi;

    if(a >= 1.0f)
        return 0.0f;

    return acosf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float atan(float a)
{
    return atanf(a);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float atan(float const y, float const x)
{
    return atan2f(y, x);
}

//-----------------------------------------------------------------------------------------------------------
/**
 * 4th order polynomial approximation
 * 4 VGRP, 16 ALU Full Rate
 * 7 * 10^-5 radians precision
 * Reference : Handbook of Mathematical Functions 
 * (chapter : Elementary Transcendental Functions), M. Abramowitz and I.A. Stegun, Ed.
 */
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

//-----------------------------------------------------------------------------------------------------------
/**
 * 4th order polynomial approximation
 * 4 VGRP, 16 ALU Full Rate
 * 7 * 10^-5 radians precision 
 */
inline float
asin_fast4(float const in)
{
    auto const x = in;

    // asin is offset of acos
    return pi_div_2 - acos_fast4(x);
}

//-----------------------------------------------------------------------------------------------------------
/**
 * 4th order hyperbolical approximation
 * 4 VGRP, 12 ALU Full Rate
 * 7 * 10^-5 radians precision 
 * Reference : Efficient approximations for the arctangent function,
 * Rajan, S. Sichun Wang Inkol, R. Joyal, A., May 2006
 */
inline float
atan_fast4(float const in)
{
    auto const x = in;
    return x * (-0.1784f * std::abs(x) - 0.0663f * x * x + 1.0301f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float logn(float f)
{
    return ::logf(f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float pow(float x, float y)
{
    return ::powf(x, y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float exp(float f)
{
    return ::expf(f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
pow_integral(int32_t const x, int32_t y) XR_NOEXCEPT
{
    auto r = 0;

    for(r = x; y > 1; y--)
        r *= x;

    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int32_t logn2_integral(float f) XR_NOEXCEPT
{
    return (((*reinterpret_cast<int32_t*>(&f)) >> ieee_flt_mantissa_bits)& ((1 << ieee_flt_exponent_bits) - 1)) - ieee_flt_exponent_bias;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int32_t
logn2_integral(int32_t const i) XR_NOEXCEPT
{
    return logn2_integral(static_cast<float>(i));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int32_t
bits_for_float(float const f) XR_NOEXCEPT
{
    return logn2_integral(f) + 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int32_t
bits_for_integer(int32_t const i) XR_NOEXCEPT
{
    return logn2_integral(static_cast<float>(i)) + 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int32_t
mask_for_float_sign(float f) XR_NOEXCEPT
{
    return (static_cast<int32_t>(f) >> 31);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
mask_for_integer_sign(int32_t i) XR_NOEXCEPT
{
    return (i >> 31);
}

//-----------------------------------------------------------------------------------------------------------
inline int32_t ifloor(float x)
{
    int32_t a = static_cast<int32_t>(x);
    int32_t exponent = (127 + 31) - ((a >> 23) & 0xFF);
    int32_t r = (((uint32_t)(a) << 8) | (1U << 31)) >> exponent;
    exponent += 31 - 127;
    {
        int32_t imask = (!(((((1 << (exponent))) - 1) >> 8)& a));
        exponent -= (31 - 127) + 32;
        exponent >>= 31;
        a >>= 31;
        r -= (imask & a);
        r &= exponent;
        r ^= a;
    }
    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
ceil_power_of_two(int32_t x) XR_NOEXCEPT
{
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
floor_power_of_two(int32_t x) XR_NOEXCEPT
{
    return ceil_power_of_two(x) >> 1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool
is_power_of_two(int32_t const x) XR_NOEXCEPT
{
    return (x & (x - 1)) == 0 && x > 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
bit_count(int32_t x) XR_NOEXCEPT
{
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    return ((x + (x >> 16)) & 0x0000003f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
bit_reverse(int32_t x) XR_NOEXCEPT
{
    x = (((x >> 1) & 0x55555555) | ((x & 0x55555555) << 1));
    x = (((x >> 2) & 0x33333333) | ((x & 0x33333333) << 2));
    x = (((x >> 4) & 0x0f0f0f0f) | ((x & 0x0f0f0f0f) << 4));
    x = (((x >> 8) & 0x00ff00ff) | ((x & 0x00ff00ff) << 8));
    return ((x >> 16) | (x << 16));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE int32_t
absolute(int32_t const x) XR_NOEXCEPT
{
    auto const y = x >> 31;
    return ((x ^ y) - y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
absolute_float(float f) XR_NOEXCEPT
{
    auto tmp = *reinterpret_cast<int32_t*>(&f);
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float*>(&tmp);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float floor(float f)
{
    return ::floorf(f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float ceil(float f)
{
    return ::ceilf(f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float nearest_integer(float const f)
{
    return floorf(f + 0.5f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float angle_normalize360(float angle)
{
    if((angle >= 360.0f) || (angle < 0.0f))
        angle -= ::floorf(angle / 360.0f) * 360.0f;

    return angle;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float angle_normalize180(float angle)
{
    angle = angle_normalize360(angle);
    if(angle > 180.0f)
        angle -= 360.0f;

    return angle;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float
angle_delta(float const angle1, float const angle2)
{
    return angle_normalize180(angle1 - angle2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline int32_t
float_hash(float const* array, size_t const num_floats) XR_NOEXCEPT
{
    int32_t hash = 0;
    int32_t const* ptr = reinterpret_cast<int32_t const*>(array);

    for(size_t i = 0; i < num_floats; i++)
        hash ^= ptr[i];

    return hash;
}

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
