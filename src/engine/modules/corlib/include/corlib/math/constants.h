// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

// float values defines
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_mask_sign_bit = UINT32_C(0x080000000); // mask for sign bit
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_mask_abs_value = UINT32_C(0x07FFFFFFF); // mask for absolute value (~sgn)
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_mantissa = UINT32_C(0x0007FFFFF); // mask for mantissa
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_mask_exponent = UINT32_C(0x07F800000); // mask for exponent
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_one_value = UINT32_C(0x03F800000); // 1.0f
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_half_value = UINT32_C(0x03F000000); // 0.5f
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_two_value = UINT32_C(0x040000000); // 2.0
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_out_of_bounds = UINT32_C(0x000000000); // "out of bounds" value
uint32_t XR_CONSTEXPR_CPP14_OR_CONST float_rle10 = UINT32_C(0x03ede5bdb); // 1/ln10

uint32_t XR_CONSTEXPR_CPP14_OR_CONST max_uint32 = UINT32_MAX;
int32_t XR_CONSTEXPR_CPP14_OR_CONST min_int32 = 0;
int32_t XR_CONSTEXPR_CPP14_OR_CONST max_int32 = INT32_MAX;
float const min_float = 1.175494351e-38F;
float const max_float = 3.402823466e+38F;
double const min_double = 2.2250738585072014e-308;
double const max_double = 1.7976931348623158e+308;
double XR_CONSTEXPR_CPP14_OR_CONST min_pos_double = 1.175494351e-38;
float XR_CONSTEXPR_CPP14_OR_CONST min_pos_float = static_cast<float>(min_pos_double);

float XR_CONSTEXPR_CPP14_OR_CONST pi = 3.1415926535897932384626433832795f;
float XR_CONSTEXPR_CPP14_OR_CONST two_pi = 6.2831853071795864769252867665590f;
float XR_CONSTEXPR_CPP14_OR_CONST three_pi = 9.4247779607693797153879301498385f;
float XR_CONSTEXPR_CPP14_OR_CONST four_pi = 12.566370614359172953850573533118f;
float XR_CONSTEXPR_CPP14_OR_CONST six_pi = 18.849555921538759430775860299677f;
float XR_CONSTEXPR_CPP14_OR_CONST eight_pi = 25.132741228718345907701147066236f;
float XR_CONSTEXPR_CPP14_OR_CONST pi_recip = 0.318309886f;
float XR_CONSTEXPR_CPP14_OR_CONST two_pi_recip = 0.159154943f;
float XR_CONSTEXPR_CPP14_OR_CONST pi_div_2 = 1.5707963267948966192313216916398f;
float XR_CONSTEXPR_CPP14_OR_CONST pi_div_3 = 1.0471975511965977461542144610932f;
float XR_CONSTEXPR_CPP14_OR_CONST pi_div_4 = 0.7853981633974483096156608458199f;
float XR_CONSTEXPR_CPP14_OR_CONST pi_div_6 = 0.5235987755982988730771072305466f;
float XR_CONSTEXPR_CPP14_OR_CONST pi_div_8 = 0.3926990816987241548078304229099f;

float XR_CONSTEXPR_CPP14_OR_CONST log_nat_base10 = 2.3025850929940456840179914546844f;
float XR_CONSTEXPR_CPP14_OR_CONST inv_log_nat_base2 = 1.4426950408889634073599246810019f;
float XR_CONSTEXPR_CPP14_OR_CONST log_nat_base2_hi = 0.6931471805599453094172321214582f;
float XR_CONSTEXPR_CPP14_OR_CONST log_nat_base2_lo = 1.90821492927058770002e-10f;

float XR_CONSTEXPR_CPP14_OR_CONST eps = 0.000001f;
float XR_CONSTEXPR_CPP14_OR_CONST eps_s = 0.0000001f;
float XR_CONSTEXPR_CPP14_OR_CONST eps_l = 0.0010000f;

// very small epsilon for checking against 0.0f
float XR_CONSTEXPR_CPP14_OR_CONST near_zero_eps = (32.0f * min_pos_float);
// alternative small epsilon for checking against 0.0f
float XR_CONSTEXPR_CPP14_OR_CONST near_zero_eps_alt = 1.0f / float(1 << 28);

double XR_CONSTEXPR_CPP14_OR_CONST huge_number = 1e+300;
float XR_CONSTEXPR_CPP14_OR_CONST infinity = ((float)(huge_number * huge_number));
float XR_CONSTEXPR_CPP14_OR_CONST nan = ((float)(infinity * 0.0F));

uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_flt_mantissa_bits = 23;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_flt_exponent_bits = 8;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_flt_exponent_bias = 127;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_flt_sign_bit = 31;

uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_dbl_mantissa_bits = 52;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_dbl_exponent_bits = 11;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_dbl_exponent_bias = 1023;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_dbl_sign_bit = 63;

uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_double_mantissa_bit = 63;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_double_exponent_bits = 15;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_double_exponent_bias = 0;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_double_sign_bit = 79;

// Derived from batch testing
// TODO : Should be improved
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_const_nr0 = 0x7EF311C2;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_const_nr1 = 0x7EF311C3;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_const_nr2 = 0x7EF312AC;

// Derived from batch testing
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_sqrt_const_nr0 = 0x1FBD1DF5;

// Biases for global ranges
// 0-1 or 1-2 specific ranges might improve from different bias
// Derived from batch testing
// TODO : Should be improved
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_sqrt_const_nr0 = 0x5f3759df;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_sqrt_const_nr1 = 0x5F375A86;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_sqrt_const_nr2 = 0x5F375A86;

//
// Normalized range [0,1] Constants
//
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_const_nr0_snorm = 0x7EEF370B;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_sqrt_const_nr0_snorm = 0x1FBD1DF5;
uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_sqrt_const_nr0_snorm = 0x5F341A43;

//
// Distance [0,1000] based constants
//
//uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_const_nr0_snorm = 0x7EF3210C;
//uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_sqrt_const_nr0_snorm = 0x1FBD22DF;
//uint32_t XR_CONSTEXPR_CPP14_OR_CONST ieee_int_rcp_sqrt_const_nr0_snorm = 0x5F33E79F;

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
