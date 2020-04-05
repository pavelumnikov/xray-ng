#pragma once

#include "corlib/types.h"
#include "EASTL/numeric_limits.h"
#include <cmath>

//------------------------------------------------------------------------------
namespace xr::math
{

// float values defines
auto constexpr float_mask_sign_bit = 0x080000000; // mask for sign bit
auto constexpr float_mask_abs_value = 0x07FFFFFFF; // mask for absolute value (~sgn)
auto constexpr float_mantissa = 0x0007FFFFF; // mask for mantissa
auto constexpr float_mask_exponent = 0x07F800000; // mask for exponent
auto constexpr float_one_value = 0x03F800000; // 1.0f
auto constexpr float_half_value = 0x03F000000; // 0.5f
auto constexpr float_two_value = 0x040000000; // 2.0
auto constexpr float_out_of_bounds = 0x000000000; // "out of bounds" value
auto constexpr float_rle10 = 0x03ede5bdb; // 1/ln10

auto constexpr max_uint32 = eastl::numeric_limits<uint32_t>::max();
auto constexpr min_int32 = eastl::numeric_limits<int32_t>::min();
auto constexpr max_int32 = eastl::numeric_limits<int32_t>::max();
auto const min_float = eastl::numeric_limits<float>::min();
auto const max_float = eastl::numeric_limits<float>::max();
auto const min_double = eastl::numeric_limits<double>::min();
auto const max_double = eastl::numeric_limits<double>::max();
auto constexpr min_pos_double = 1.175494351e-38;
auto constexpr min_pos_float = static_cast<float>(min_pos_double);

auto constexpr pi = 3.1415926535897932384626433832795f;
auto constexpr two_pi = 6.2831853071795864769252867665590f;
auto constexpr three_pi = 9.4247779607693797153879301498385f;
auto constexpr four_pi = 12.566370614359172953850573533118f;
auto constexpr six_pi = 18.849555921538759430775860299677f;
auto constexpr eight_pi = 25.132741228718345907701147066236f;
auto constexpr pi_recip = 0.318309886f;
auto constexpr two_pi_recip = 0.159154943f;
auto constexpr pi_div_2 = 1.5707963267948966192313216916398f;
auto constexpr pi_div_3 = 1.0471975511965977461542144610932f;
auto constexpr pi_div_4 = 0.7853981633974483096156608458199f;
auto constexpr pi_div_6 = 0.5235987755982988730771072305466f;
auto constexpr pi_div_8 = 0.3926990816987241548078304229099f;

auto constexpr eps = 0.000001f;
auto constexpr eps_s = 0.0000001f;
auto constexpr eps_l = 0.0010000f;

// Very small epsilon for checking against 0.0f
auto constexpr zero_eps = (32.0f * min_pos_float);
auto const nan = eastl::numeric_limits<float>::quiet_NaN();

auto constexpr ieee_flt_mantissa_bits = 23;
auto constexpr ieee_flt_exponent_bits = 8;
auto constexpr ieee_flt_exponent_bias = 127;
auto constexpr ieee_flt_sign_bit = 31;

auto constexpr ieee_dbl_mantissa_bits = 52;
auto constexpr ieee_dbl_exponent_bits = 11;
auto constexpr ieee_dbl_exponent_bias = 1023;
auto constexpr ieee_dbl_sign_bit = 63;

auto constexpr ieee_double_mantissa_bit = 63;
auto constexpr ieee_double_exponent_bits = 15;
auto constexpr ieee_double_exponent_bias = 0;
auto constexpr ieee_double_sign_bit = 79;

// Derived from batch testing
// TODO : Should be improved
auto constexpr ieee_int_rcp_const_nr0 = 0x7EF311C2;
auto constexpr ieee_int_rcp_const_nr1 = 0x7EF311C3;
auto constexpr ieee_int_rcp_const_nr2 = 0x7EF312AC;

// Derived from batch testing
auto constexpr ieee_int_sqrt_const_nr0 = 0x1FBD1DF5;

// Biases for global ranges
// 0-1 or 1-2 specific ranges might improve from different bias
// Derived from batch testing
// TODO : Should be improved
auto constexpr ieee_int_rcp_sqrt_const_nr0 = 0x5f3759df;
auto constexpr ieee_int_rcp_sqrt_const_nr1 = 0x5F375A86;
auto constexpr ieee_int_rcp_sqrt_const_nr2 = 0x5F375A86;

//
// Normalized range [0,1] Constants
//
auto constexpr ieee_int_rcp_const_nr0_snorm = 0x7EEF370B;
auto constexpr ieee_int_sqrt_const_nr0_snorm = 0x1FBD1DF5;
auto constexpr ieee_int_rcp_sqrt_const_nr0_snorm = 0x5F341A43;

//
// Distance [0,1000] based constants
//
//auto constexpr ieee_int_rcp_const_nr0_snorm = 0x7EF3210C;
//auto constexpr ieee_int_sqrt_const_nr0_snorm = 0x1FBD22DF;
//auto constexpr ieee_int_rcp_sqrt_const_nr0_snorm = 0x5F33E79F;

inline auto float_sign_bit_set(float x) noexcept
{
    return (static_cast<const unsigned long>(x) >> 31);
}

inline auto float_is_nan(float x) noexcept
{
    return (((static_cast<const unsigned long>(x)) & 0x7f800000) == 0x7f800000);
}

inline auto float_is_inf(float x) noexcept
{
    return (((static_cast<const unsigned long>(x)) & 0x7fffffff) == 0x7f800000);
}

inline auto float_is_ind(float x) noexcept
{
    return ((static_cast<const unsigned long>(x)) == 0xffc00000);
}

inline auto float_is_denormal(float x) noexcept
{
    return (((static_cast<const unsigned long>(x)) & 0x7f800000) == 0x00000000 &&
        ((static_cast<const unsigned long>(x)) & 0x007fffff) != 0x00000000);
};

template<class T>
constexpr int max_index(T x, T y) noexcept
{
    return  (x > y) ? 0 : 1;
}

template<class T>
constexpr int min_index(T x, T y) noexcept
{
    return (x < y) ? 0 : 1;
}

template<class T>
constexpr T max3(T x, T y, T z) noexcept
{
    return (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
}

template<class T>
constexpr T min3(T x, T y, T z) noexcept
{
    return (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
}

template<class T>
constexpr int max3_index(T x, T y, T z) noexcept
{
    return (x > y) ? ((x > z) ? 0 : 2) : ((y > z) ? 1 : 2);
}

template<class T>
constexpr int min3_index(T x, T y, T z) noexcept
{
    return (x < y) ? ((x < z) ? 0 : 2) : ((y < z) ? 1 : 2);
}

template<class T>
constexpr T sign(T f)
{
    return (f > 0) ? 1 : ((f < 0) ? -1 : 0);
}

template<class T>
constexpr T square(T x)
{
    return x * x;
}

template<class T> constexpr T cube(T x)
{
    return x * x * x;
}

enum
{
    LOOKUP_BITS = 8,
    EXP_POS = 23,
    EXP_BIAS = 127,
    LOOKUP_POS = (EXP_POS - LOOKUP_BITS),
    SEED_POS = (EXP_POS - 8),
    SQRT_TABLE_SIZE = (2 << LOOKUP_BITS),
    LOOKUP_MASK = (SQRT_TABLE_SIZE - 1)
};

//------------------------------------------------------------------------------
inline bool negative(const float& f)
{
    return (*((unsigned*)(&f)) & float_mask_sign_bit);
}

//------------------------------------------------------------------------------
inline bool positive(const float& f)
{
    return (*((unsigned*)(&f)) & float_mask_sign_bit) == 0;
}

//------------------------------------------------------------------------------
inline void set_negative(float& f)
{
    (*(unsigned*)(&f)) |= float_mask_sign_bit;
}

//------------------------------------------------------------------------------
inline void set_positive(float& f)
{
    (*(unsigned*)(&f)) &= ~float_mask_sign_bit;
}

//------------------------------------------------------------------------------
template<typename T>
constexpr int32_t
approx_as_int(T const value) noexcept
{
    return static_cast<int32_t>(value);
}

//------------------------------------------------------------------------------
template<typename T>
constexpr float
approx_as_float(T const value) noexcept
{
    return static_cast<float>(value);
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
    auto rcp = rcp_sqrt_ieee_int_approx(in, ieee_int_rcp_sqrt_const_nr1);
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
    auto rcp = rcp_sqrt_ieee_int_approx(in, ieee_int_rcp_sqrt_const_nr2);
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
    float  rcp = sqrt_ieee_int_approx(in, ieee_int_sqrt_const_nr0);
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
    auto const rcp = rcp_ieee_int_approx(in, ieee_int_rcp_const_nr0);
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
    auto rcp = rcp_ieee_int_approx(in, ieee_int_rcp_const_nr1);
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
    auto rcp = rcp_ieee_int_approx(in, ieee_int_rcp_const_nr2);
    rcp = rcp_newton_raphson(in, rcp);
    rcp = rcp_newton_raphson(in, rcp);

    return rcp;
}

inline float
rsqrt(float x) noexcept
{
    return fast_rcp_sqrt_nr2(x);
}

inline float
inv_sqrt(float x)
{
    return 1 / fast_rcp_sqrt_nr2(x);
}

inline float
sqrt(float x)
{
    return x * inv_sqrt(x);
}

inline float
sine(float a)
{
    return sinf(a);
}

inline float
sine16(float a)
{
    if((a < 0.0f) || (a >= two_pi))
        a -= floorf(a / two_pi) * two_pi;

    if(a < pi)
    {
        if(a > pi_div_2)
            a = pi - a;
    }
    else
    {
        if(a > pi + pi_div_2)
            a = a - two_pi;
        else
            a = pi - a;
    }

    const auto s = a * a;
    return a * (((((-2.39e-08f * s + 2.7526e-06f) * s - 1.98409e-04f) * s + 8.3333315e-03f) * s - 1.666666664e-01f) * s + 1.0f);
}

inline double
sine64(float a)
{
    return ::sin(static_cast<double>(a));
}

inline float
cosine(float a)
{
    return ::cosf(a);
}

inline float
cosine16(float a)
{
    auto d { 0.f };

    if((a < 0.0f) || (a >= two_pi))
        a -= floorf(a / two_pi) * two_pi;

    if(a < pi)
    {
        if(a > pi_div_2)
        {
            a = pi - a;
            d = -1.0f;
        }
        else
        {
            d = 1.0f;
        }
    }
    else
    {
        if(a > pi + pi_div_2)
        {
            a = a - two_pi;
            d = 1.0f;
        }
        else
        {
            a = pi - a;
            d = -1.0f;
        }
    }

    const auto s = a * a;
    return d * (((((-2.605e-07f * s + 2.47609e-05f) * s - 1.3888397e-03f) * s + 4.16666418e-02f) * s - 4.999999963e-01f) * s + 1.0f);
}

inline double
cosine64(float a)
{
    return ::cos(static_cast<double>(a));
}

inline void
sin_cos(float a, float& s, float& c)
{
    s = ::sinf(a);
    c = ::cosf(a);
}

inline void
sin_cos16(float a, float& s, float& c)
{
    auto d = 0.f;

    if((a < 0.0f) || (a >= two_pi))
        a -= floorf(a / two_pi) * two_pi;

    if(a < pi)
    {
        if(a > pi_div_2)
        {
            a = pi - a;
            d = -1.0f;
        }
        else
        {
            d = 1.0f;
        }
    }
    else
    {
        if(a > pi + pi_div_2)
        {
            a = a - two_pi;
            d = 1.0f;
        }
        else
        {
            a = pi - a;
            d = -1.0f;
        }
    }

    auto const t = a * a;
    s = a * (((((-2.39e-08f * t + 2.7526e-06f) * t - 1.98409e-04f) * t + 8.3333315e-03f) * t - 1.666666664e-01f) * t + 1.0f);
    c = d * (((((-2.605e-07f * t + 2.47609e-05f) * t - 1.3888397e-03f) * t + 4.16666418e-02f) * t - 4.999999963e-01f) * t + 1.0f);
}

inline void
sin_cos64(float const a, double& s, double& c)
{
    const auto f = static_cast<double>(a);
    s = ::sin(f);
    c = ::cos(f);
}

inline float
tan(float a)
{
    return ::tanf(a);
}

inline float
tan16(float a)
{
    auto reciprocal = false;

    if((a < 0.0f) || (a >= pi))
        a -= ::floorf(a / pi) * pi;

    if(a < pi_div_2)
    {
        if(a > pi_div_4)
        {
            a = pi_div_2 - a;
            reciprocal = true;
        }

        // reciprocal already false, so we don't need to set "false" here
    }
    else
    {
        if(a > pi_div_2 + pi_div_4)
        {
            a = a - pi;
            // reciprocal already false, so we don't need to set "false" here
        }
        else
        {
            a = pi_div_2 - a;
            reciprocal = true;
        }
    }

    auto s = a * a;
    s = a * ((((((9.5168091e-03f * s + 2.900525e-03f) * s + 2.45650893e-02f) * s + 5.33740603e-02f) * s + 1.333923995e-01f) * s + 3.333314036e-01f) * s + 1.0f);

    if(reciprocal)
        return 1.0f / s;

    return s;
}

inline double
tan64(float const a)
{
    return ::tan(static_cast<double>(a));
}

inline float
asin(float const a)
{
    if(a <= -1.0f)
        return -pi_div_2;

    if(a >= 1.0f)
        return pi_div_2;

    return ::asinf(a);
}

inline float
asin16(float a)
{
    if(float_sign_bit_set(a))
    {
        if(a <= -1.0f)
            return -pi_div_2;

        a = fabsf(a);
        return (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrtf(1.0f - a) - pi_div_2;
    }

    if(a >= 1.0f)
        return pi_div_2;

    return pi_div_2 - (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrtf(1.0f - a);
}

inline double
asin64(float a)
{
    if(a <= -1.0f)
        return -pi_div_2;

    if(a >= 1.0f)
        return pi_div_2;

    return ::asin(static_cast<double>(a));
}

inline float
acos(float a)
{
    if(a <= -1.0f)
        return pi;

    if(a >= 1.0f)
        return 0.0f;

    return acosf(a);
}

inline float acos16(float a)
{
    if(float_sign_bit_set(a))
    {
        if(a <= -1.0f)
            return pi;

        a = fabsf(a);

        return pi - (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrtf(1.0f - a);
    }

    if(a >= 1.0f)
        return 0.0f;

    return (((-0.0187293f * a + 0.0742610f) * a - 0.2121144f) * a + 1.5707288f) * sqrtf(1.0f - a);
}

inline double acos64(float const a)
{
    if(a <= -1.0f)
        return pi;

    if(a >= 1.0f)
        return 0.0f;

    return ::acos(static_cast<double>(a));
}

inline float atan(float a)
{
    return atanf(a);
}

inline float atan16(float a)
{
    if(::fabs(a) > 1.0f)
    {
        a = 1.0f / a;
        auto s = a * a;
        s = -(((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
            * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
        if(float_sign_bit_set(a))
            return s - pi_div_2;
        else
            return s + pi_div_2;
    }

    auto const s = a * a;
    return (((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
        * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
}

inline double atan64(float const a)
{
    return ::atan(static_cast<double>(a));
}

inline float atan(float const y, float const x)
{
    return atan2f(y, x);
}

inline float
atan16(float const y, float const x)
{
    if(::fabsf(y) > ::fabsf(x))
    {
        auto a = x / y;
        auto s = a * a;
        s = -(((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
            * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
        if(float_sign_bit_set(a))
            return s - pi_div_2;
        else
            return s + pi_div_2;
    }


    auto const a = y / x;
    auto const s = a * a;
    return (((((((((0.0028662257f * s - 0.0161657367f) * s + 0.0429096138f) * s - 0.0752896400f)
        * s + 0.1065626393f) * s - 0.1420889944f) * s + 0.1999355085f) * s - 0.3333314528f) * s) + 1.0f) * a;
}

inline double
atan64(float const y, float const x)
{
    return ::atan2(static_cast<double>(y), static_cast<double>(x));
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

inline float logn(float f)
{
    return ::logf(f);
}

inline float logn16(float f) noexcept
{
    auto i = *reinterpret_cast<int*>(&f);
    auto const exponent = ((i >> ieee_flt_mantissa_bits)& ((1 << ieee_flt_exponent_bits) - 1)) - ieee_flt_exponent_bias;
    i -= (exponent + 1) << ieee_flt_mantissa_bits; // get value in the range [.5, 1>
    auto y = *reinterpret_cast<float*>(&i);
    y *= 1.4142135623730950488f; // multiply with sqrt( 2 )
    y = (y - 1.0f) / (y + 1.0f);
    auto const y2 = y * y;
    y = y * (2.000000000046727f + y2 * (0.666666635059382f + y2 * (0.4000059794795f + y2 * (0.28525381498f + y2 * 0.2376245609f))));
    y += 0.693147180559945f * (static_cast<float>(exponent) + 0.5f);
    return y;
}

inline double logn64(float f)
{
    return ::log(static_cast<double>(f));
}

inline float
pow(float x, float y)
{
    return ::powf(x, y);
}

inline float exp(float f)
{
    return ::expf(f);
}

inline float exp16(float f) noexcept
{
    auto x = f * 1.44269504088896340f; // multiply with ( 1 / log( 2 ) )
    auto i = *reinterpret_cast<int*>(&x);
    const auto s = (i >> ieee_flt_sign_bit);
    const auto e = ((i >> ieee_flt_mantissa_bits)& ((1 << ieee_flt_exponent_bits) - 1)) - ieee_flt_exponent_bias;
    const auto m = (i & ((1 << ieee_flt_mantissa_bits) - 1)) | (1 << ieee_flt_mantissa_bits);
    i = ((m >> (ieee_flt_mantissa_bits - e)) & ~(e >> 31)) ^ s;

    auto exponent = static_cast<float>((i + ieee_flt_exponent_bias) << ieee_flt_mantissa_bits);
    auto y = *reinterpret_cast<float*>(&exponent);
    x -= static_cast<float>(i);

    if(x >= 0.5f)
    {
        x -= 0.5f;
        y *= 1.4142135623730950488f;	// multiply with sqrt( 2 )
    }

    auto const x2 = x * x;
    auto const p = x * (7.2152891511493f + x2 * 0.0576900723731f);
    auto const q = 20.8189237930062f + x2;
    x = y * (q + p) / (q - p);
    return x;
}

inline double exp64(float f)
{
    return ::exp(static_cast<double>(f));
}

inline float
pow16(float x, float y)
{
    return exp16(y * logn16(x));
}

inline double
pow64(float x, float y)
{
    return ::pow(static_cast<double>(x), static_cast<double>(y));
}

constexpr int pow_integral(int const x, int y) noexcept
{
    auto r = 0;

    for(r = x; y > 1; y--)
        r *= x;

    return r;
}

inline int
logn2_integral(float f) noexcept
{
    return (((*reinterpret_cast<int*>(&f)) >> ieee_flt_mantissa_bits)& ((1 << ieee_flt_exponent_bits) - 1)) - ieee_flt_exponent_bias;
}

inline int
logn2_integral(int const i) noexcept
{
    return logn2_integral(static_cast<float>(i));
}

inline int
bits_for_float(float const f) noexcept
{
    return logn2_integral(f) + 1;
}

inline int
bits_for_integer(int const i) noexcept
{
    return logn2_integral(static_cast<float>(i)) + 1;
}

inline int
mask_for_float_sign(float f) noexcept
{
    return ((*reinterpret_cast<int*>(&f)) >> 31);
}

constexpr int
mask_for_integer_sign(int i) noexcept
{
    return (i >> 31);
}

//------------------------------------------------------------------------------
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

constexpr int
ceil_power_of_two(int x) noexcept
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

constexpr int
floor_power_of_two(int x) noexcept
{
    return ceil_power_of_two(x) >> 1;
}

constexpr bool
is_power_of_two(int const x) noexcept
{
    return (x & (x - 1)) == 0 && x > 0;
}

constexpr int
bit_count(int x) noexcept
{
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    return ((x + (x >> 16)) & 0x0000003f);
}

constexpr int
bit_reverse(int x) noexcept
{
    x = (((x >> 1) & 0x55555555) | ((x & 0x55555555) << 1));
    x = (((x >> 2) & 0x33333333) | ((x & 0x33333333) << 2));
    x = (((x >> 4) & 0x0f0f0f0f) | ((x & 0x0f0f0f0f) << 4));
    x = (((x >> 8) & 0x00ff00ff) | ((x & 0x00ff00ff) << 8));
    return ((x >> 16) | (x << 16));
}

constexpr int
absolute(int const x) noexcept
{
    auto const y = x >> 31;
    return ((x ^ y) - y);
}

inline float
absolute_float(float f) noexcept
{
    auto tmp = *reinterpret_cast<int*>(&f);
    tmp &= 0x7FFFFFFF;
    return *reinterpret_cast<float*>(&tmp);
}

inline float  floor(float f)
{
    return ::floorf(f);
}

inline float ceil(float f)
{
    return ::ceilf(f);
}

inline float nearest_integer(float const f)
{
    return floorf(f + 0.5f);
}

constexpr int8_t
clamp8(int32_t const i) noexcept
{
    if(i < -128)
        return -128;

    if(i > 127)
        return 127;

    return static_cast<int8_t>(i);
}

constexpr int16_t
clamp16(int32_t const i) noexcept
{
    if(i < -32768)
        return -32768;

    if(i > 32767)
        return 32767;

    return static_cast<int16_t>(i);
}

constexpr int
clamp32(int32_t const min, int32_t const max, int32_t const value) noexcept
{
    if(value < min)
        return min;

    if(value > max)
        return max;

    return value;
}

constexpr float
clamp_float(float const min, float const max, float const value) noexcept
{
    if(value < min)
        return min;

    if(value > max)
        return max;

    return value;
}

inline float angle_normalize360(float angle)
{
    if((angle >= 360.0f) || (angle < 0.0f))
        angle -= ::floorf(angle / 360.0f) * 360.0f;

    return angle;
}

inline float angle_normalize180(float angle)
{
    angle = angle_normalize360(angle);
    if(angle > 180.0f)
        angle -= 360.0f;

    return angle;
}

inline float
angle_delta(float const angle1, float const angle2)
{
    return angle_normalize180(angle1 - angle2);
}

inline int
float_hash(const float* array, const int num_floats)noexcept
{
    auto hash = 0;
    auto const ptr = reinterpret_cast<const int*>(array);

    for(auto i = 0; i < num_floats; i++)
        hash ^= ptr[i];

    return hash;
}

} // namespace xr::math
//------------------------------------------------------------------------------