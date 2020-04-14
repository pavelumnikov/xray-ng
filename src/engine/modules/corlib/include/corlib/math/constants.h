// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/numeric_limits.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::math
{

// float values defines
auto constexpr float_mask_sign_bit = UINT32_C(0x080000000); // mask for sign bit
auto constexpr float_mask_abs_value = UINT32_C(0x07FFFFFFF); // mask for absolute value (~sgn)
auto constexpr float_mantissa = UINT32_C(0x0007FFFFF); // mask for mantissa
auto constexpr float_mask_exponent = UINT32_C(0x07F800000); // mask for exponent
auto constexpr float_one_value = UINT32_C(0x03F800000); // 1.0f
auto constexpr float_half_value = UINT32_C(0x03F000000); // 0.5f
auto constexpr float_two_value = UINT32_C(0x040000000); // 2.0
auto constexpr float_out_of_bounds = UINT32_C(0x000000000); // "out of bounds" value
auto constexpr float_rle10 = UINT32_C(0x03ede5bdb); // 1/ln10

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

auto constexpr log_nat_base10 = 2.3025850929940456840179914546844f;
auto constexpr inv_log_nat_base2 = 1.4426950408889634073599246810019f;
auto constexpr log_nat_base2_hi = 0.6931471805599453094172321214582f;
auto constexpr log_nat_base2_lo = 1.90821492927058770002e-10f;

auto constexpr eps = 0.000001f;
auto constexpr eps_s = 0.0000001f;
auto constexpr eps_l = 0.0010000f;

// very small epsilon for checking against 0.0f
auto constexpr near_zero_eps = (32.0f * min_pos_float);
// alternative small epsilon for checking against 0.0f
auto constexpr near_zero_eps_alt = 1.0f / float(1 << 28);

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

} // namespace xr::math
//-----------------------------------------------------------------------------------------------------------
