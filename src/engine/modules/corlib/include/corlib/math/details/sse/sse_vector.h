// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/details/sse/sse_float_in_vec.h"
#include "corlib/etl/algorithms/aligning.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::math::details
{

//-----------------------------------------------------------------------------------------------------------
class XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) sse_vector
{
public:
    sse_vector();
    sse_vector(__m128 rhs);
    sse_vector(const sse_vector & rhs);
    sse_vector(_In_count_c_(4) const float* pFloats);
    sse_vector(float x, float y, float z, float w = 1.0f);
    sse_vector(const float_in_vec & x, const float_in_vec & y, const float_in_vec & z, const float_in_vec & w = float_in_vec(1.0f));

    sse_vector& XR_VECTORIZED_CALL operator = (const sse_vector & rhs);

    // Flip sign
    sse_vector XR_VECTORIZED_CALL operator - () const;

    sse_vector& XR_VECTORIZED_CALL operator += (const sse_vector & rhs);
    sse_vector& XR_VECTORIZED_CALL operator -= (const sse_vector & rhs);
    sse_vector& XR_VECTORIZED_CALL operator *= (float f);
    sse_vector& XR_VECTORIZED_CALL operator *= (const sse_vector & rhs);
    sse_vector& XR_VECTORIZED_CALL operator /= (const sse_vector & rhs);

    const sse_vector XR_VECTORIZED_CALL operator + (const sse_vector & b) const;
    const sse_vector XR_VECTORIZED_CALL operator - (const sse_vector & b) const;
    const sse_vector XR_VECTORIZED_CALL operator * (const sse_vector & b) const;
    const sse_vector XR_VECTORIZED_CALL operator * (float f) const;
    const sse_vector XR_VECTORIZED_CALL operator / (const sse_vector & b) const;

    void XR_VECTORIZED_CALL set_x(const float_in_vec & scalar);
    void XR_VECTORIZED_CALL set_x(float scalar);
    void XR_VECTORIZED_CALL set_y(const float_in_vec & scalar);
    void XR_VECTORIZED_CALL set_y(float scalar);
    void XR_VECTORIZED_CALL set_z(const float_in_vec & scalar);
    void XR_VECTORIZED_CALL set_z(float scalar);
    void XR_VECTORIZED_CALL set_w(const float_in_vec & scalar);
    void XR_VECTORIZED_CALL set_w(float scalar);

    float_in_vec get_x() const;
    float_in_vec get_y() const;
    float_in_vec get_z() const;
    float_in_vec get_w() const;

    float_in_vec XR_VECTORIZED_CALL length() const;
    float_in_vec XR_VECTORIZED_CALL squared_length() const;
    float_in_vec XR_VECTORIZED_CALL dotproduct(const sse_vector & second) const;

    sse_vector XR_VECTORIZED_CALL crossproduct(const sse_vector & second) const;
    //sse_vector MultByMatrix( const sse_matrix4& second ) const;

    sse_vector XR_VECTORIZED_CALL absolute() const;

    // Approximated normalization using Newton-Raphson method
    sse_vector XR_VECTORIZED_CALL normalize() const;

    /// Clamps this vector value between other ones
    sse_vector XR_VECTORIZED_CALL clamp(const sse_vector & min, const sse_vector & max);

    /// Mixes this value with others
    sse_vector XR_VECTORIZED_CALL mix(const sse_vector & v1, const sse_vector & v2);

    static const sse_vector lerp(const float_in_vec & dt, const sse_vector & v1, const sse_vector & v2);
    static const sse_vector lerp(float dt, const sse_vector & v1, const sse_vector & v2);

    static const sse_vector sphere_interpolate(const float_in_vec & dt, const sse_vector & v1, const sse_vector & v2);
    static const sse_vector sphere_interpolate(float dt, const sse_vector & v1, const sse_vector & v2);

    static const sse_vector hermite_interpolate(const float_in_vec & dt, const sse_vector & v1, const sse_vector & t1, const sse_vector & v2, const sse_vector & t2);
    static const sse_vector hermite_interpolate(float dt, const sse_vector & v1, const sse_vector & t1, const sse_vector & v2, const sse_vector & t2);

    static const sse_vector catmull_interpolate(const float_in_vec & dt, const sse_vector & v1, const sse_vector & v2, const sse_vector & v3, const sse_vector & v4);
    static const sse_vector catmull_interpolate(float dt, const sse_vector & v1, const sse_vector & v2, const sse_vector & v3, const sse_vector & v4);

    static const sse_vector XR_VECTORIZED_CALL forward_vec(const sse_vector & Normal, const sse_vector & Incident, const sse_vector & Reference);
    static const sse_vector XR_VECTORIZED_CALL relect(const sse_vector & Normal, const sse_vector & Incident);

    // Comparison of xyzw components

    static bool XR_VECTORIZED_CALL equal_all(const sse_vector & a, const sse_vector & b);
    static bool XR_VECTORIZED_CALL equal_any(const sse_vector & a, const sse_vector & b);
    static bool XR_VECTORIZED_CALL near_equal(const sse_vector & a, const sse_vector & b, const float epsilon);
    static bool XR_VECTORIZED_CALL near_equal(const sse_vector & a, const sse_vector & b, const float_in_vec & epsilon);
    static bool XR_VECTORIZED_CALL less_all(const sse_vector & a, const sse_vector & b);
    static bool XR_VECTORIZED_CALL less_any(const sse_vector & a, const sse_vector & b);

    // Comparison of xyz components

    static bool XR_VECTORIZED_CALL equal_all_v3(const sse_vector & a, const sse_vector & b);
    static bool XR_VECTORIZED_CALL equal_any_v3(const sse_vector & a, const sse_vector & b);
    static bool XR_VECTORIZED_CALL near_equal_v3(const sse_vector & a, const sse_vector & b, const float epsilon);
    static bool XR_VECTORIZED_CALL near_equal_v3(const sse_vector & a, const sse_vector & b, const float_in_vec & epsilon);
    static bool XR_VECTORIZED_CALL less_all_v3(const sse_vector & a, const sse_vector & b);
    static bool XR_VECTORIZED_CALL less_any_v3(const sse_vector & a, const sse_vector & b);

private:
    friend class sse_matrix4;
    __m128 v;
}; 

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector::sse_vector()
    : v { _mm_set_ps1(0.0f) }
{
    /// Empty
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector::sse_vector(__m128 rhs)
    : v { rhs }
{
    /// Empty
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector::sse_vector(const sse_vector& rhs)
    : v { rhs.v }
{
    /// Empty
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector::sse_vector(_In_count_c_(4) const float* pFloats)
    : sse_vector {}
{
    XR_DEBUG_ASSERTION_MSG(etl::algorithms::is_aligned(pFloats, 16),
        "Input data block must be aligned to 16 bytes!");
    v = _mm_load_ps(pFloats);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector::sse_vector(float x, float y, float z, float w)
    : v { _mm_setr_ps(x, y, z, w) }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector::sse_vector(const float_in_vec& x,const float_in_vec& y,
    const float_in_vec& z, const float_in_vec& w)
{
    __m128 vTemp1 = _mm_unpacklo_ps(y, w);
    __m128 vTemp2 = _mm_unpacklo_ps(x, z);
    __m128 vResult = _mm_unpacklo_ps(vTemp2, vTemp1);
    v = vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector& XR_VECTORIZED_CALL
sse_vector::operator = (const sse_vector& rhs)
{
    __m128 vResult = rhs.v;
    v = vResult;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector XR_VECTORIZED_CALL
sse_vector::operator - () const
{
    // [Codepoet]: this is faster than subtraction method
    __m128 vResult = _mm_xor_ps(v, g_fNegativeZero);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector& XR_VECTORIZED_CALL
sse_vector::operator += (const sse_vector& rhs)
{
    __m128 vResult = _mm_add_ps(v, rhs.v);
    v = vResult;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector& XR_VECTORIZED_CALL
sse_vector::operator -= (const sse_vector& rhs)
{
    __m128 vResult = _mm_sub_ps(v, rhs.v);
    v = vResult;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector& XR_VECTORIZED_CALL
sse_vector::operator *= (float f)
{
    __m128 vSecond = _mm_set_ps1(f);
    __m128 vResult = _mm_mul_ps(v, vSecond);
    v = vResult;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector& XR_VECTORIZED_CALL
sse_vector::operator *= (const sse_vector& rhs)
{
    __m128 vResult = _mm_mul_ps(v, rhs.v);
    v = vResult;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector& XR_VECTORIZED_CALL
sse_vector::operator /= (const sse_vector& rhs)
{
    __m128 vResult = _mm_div_ps(v, rhs.v);
    v = vResult;
    return *this;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_x(const float_in_vec& scalar)
{
    __m128 vTemp = float_in_vec::select_part< GET_FVEC_FROM_X >(scalar);
    v = _mm_move_ss(v, vTemp);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_x(float scalar)
{
    __m128 vTemp = _mm_load_ss(&scalar);
    v = _mm_move_ss(v, vTemp);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_y(const float_in_vec& scalar)
{
    __m128 vTemp = float_in_vec::select_part< GET_FVEC_FROM_Y >(scalar);
    v = _mm_blend_ps(v, vTemp, 2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_y(float scalar)
{
    __m128 vTemp = _mm_load_ps1(&scalar);
    v = _mm_blend_ps(v, vTemp, 2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_z(const float_in_vec& scalar)
{
    __m128 vTemp = float_in_vec::select_part< GET_FVEC_FROM_Z >(scalar);
    v = _mm_blend_ps(v, vTemp, 4);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_z(float scalar)
{
    __m128 vTemp = _mm_load_ps1(&scalar);
    v = _mm_blend_ps(v, vTemp, 4);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_w(const float_in_vec& scalar)
{
    __m128 vTemp = float_in_vec::select_part< GET_FVEC_FROM_W >(scalar);
    v = _mm_blend_ps(v, vTemp, 8);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline void XR_VECTORIZED_CALL
sse_vector::set_w(float scalar)
{
    __m128 vTemp = _mm_load_ps1(&scalar);
    v = _mm_blend_ps(v, vTemp, 8);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec
sse_vector::get_x() const
{
    return float_in_vec::select_part< GET_FVEC_FROM_X >(v);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec
sse_vector::get_y() const
{
    return float_in_vec::select_part< GET_FVEC_FROM_Y >(v);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec
sse_vector::get_z() const
{
    return float_in_vec::select_part< GET_FVEC_FROM_Z >(v);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec
sse_vector::get_w() const
{
    return float_in_vec::select_part< GET_FVEC_FROM_W >(v);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec XR_VECTORIZED_CALL
sse_vector::length() const
{
    // [Codepoet]: Sadly, built-in Dot-Product intrinsic is slow as hell :(
    //__m128 vResult = _mm_dp_ps( v, v, 0x1f );
    //vResult = _mm_sqrt_ss( vResult );
    //return float_in_vec::select_part< GET_FVEC_FROM_X >( vResult );

    // Perform the dot product on x,y and z
    __m128 vLengthSq = _mm_mul_ps(v, v);
    // vTemp has z and y
    __m128 vTemp = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
    // x+z, y
    vLengthSq = _mm_add_ss(vLengthSq, vTemp);
    // y,y,y,y
    vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
    // x+z+y,??,??,??
    vLengthSq = _mm_add_ss(vLengthSq, vTemp);
    // Splat the length squared
    vLengthSq = _mm_shuffle_ps(vLengthSq, vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
    // Get the length
    vLengthSq = _mm_sqrt_ps(vLengthSq);
    return vLengthSq;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec XR_VECTORIZED_CALL
sse_vector::squared_length() const
{
    // Perform the dot product
    __m128 vDot = _mm_mul_ps(v, v);
    // x=Dot.vector4_f32[1], y=Dot.vector4_f32[2]
    __m128 vTemp = _mm_shuffle_ps(vDot, vDot, _MM_SHUFFLE(2, 1, 2, 1));
    // Result.vector4_f32[0] = x+y
    vDot = _mm_add_ss(vDot, vTemp);
    // x=Dot.vector4_f32[2]
    vTemp = _mm_shuffle_ps(vTemp, vTemp, _MM_SHUFFLE(1, 1, 1, 1));
    // Result.vector4_f32[0] = (x+y)+z
    vDot = _mm_add_ss(vDot, vTemp);
    // Splat x
    vDot = _mm_shuffle_ps(vDot, vDot, _MM_SHUFFLE(0, 0, 0, 0));
    return vDot;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline float_in_vec XR_VECTORIZED_CALL
sse_vector::dotproduct(const sse_vector& second) const
{
    // [Codepoet]: Sadly, built-in Dot-Product intrinsic is slow as hell :(
    //return _mm_dp_ps( v, second.v, 0xFF );

    __m128 vCross = _mm_mul_ps(v, second.v);
    __m128 vTemp1 = _mm_shuffle_ps(vCross, vCross, _MM_SHUFFLE(2, 2, 2, 2));
    __m128 vTemp2 = _mm_shuffle_ps(vCross, vCross, _MM_SHUFFLE(1, 1, 1, 1));
    vTemp1 = _mm_add_ps(vTemp2, vTemp1);
    vTemp2 = _mm_shuffle_ps(vCross, vCross, _MM_SHUFFLE(0, 0, 0, 0));
    vCross = _mm_add_ps(vTemp2, vTemp1);
    return vCross;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector XR_VECTORIZED_CALL
sse_vector::crossproduct(const sse_vector& second) const
{
    __m128 vTemp1 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 0, 2, 1));
    __m128 vTemp2 = _mm_shuffle_ps(second.v, second.v, _MM_SHUFFLE(3, 1, 0, 2));
    __m128 result = _mm_mul_ps(vTemp1, vTemp2);
    vTemp1 = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 1, 0, 2));
    vTemp2 = _mm_shuffle_ps(second.v, second.v, _MM_SHUFFLE(3, 0, 2, 1));
    vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
    result = _mm_sub_ps(result, vTemp1);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector XR_VECTORIZED_CALL
sse_vector::absolute() const
{
    __m128 vResult = _mm_setzero_ps();
    vResult = _mm_sub_ps(vResult, v);
    vResult = _mm_max_ps(vResult, v);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector XR_VECTORIZED_CALL
sse_vector::normalize() const
{
    __m128 vResult = _mm_mul_ps(v, v);
    __m128 vTemp1 = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(2, 2, 2, 2));
    __m128 vTemp2 = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(1, 1, 1, 1));
    vTemp1 = _mm_add_ps(vTemp2, vTemp1);
    vTemp2 = _mm_shuffle_ps(vResult, vResult, _MM_SHUFFLE(0, 0, 0, 0));
    vResult = _mm_add_ps(vTemp2, vTemp1);
    vResult = _mm_rsqrt_ps(vResult);
    vResult = _mm_mul_ps(v, vResult);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector XR_VECTORIZED_CALL
sse_vector::clamp(const sse_vector& min, const sse_vector& max)
{
    __m128 vTemp = _mm_min_ps(v, max.v);
    vTemp = _mm_max_ps(vTemp, min.v);
    return vTemp;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline sse_vector XR_VECTORIZED_CALL
sse_vector::mix(const sse_vector& v1, const sse_vector& v2)
{
    __m128 vTemp1 = _mm_sub_ps(g_fOne, v);
    vTemp1 = _mm_mul_ps(v1.v, vTemp1);
    __m128 vTemp2 = _mm_mul_ps(v2.v, v);
    vTemp2 = _mm_add_ps(vTemp1, vTemp2);
    return vTemp2;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector
sse_vector::lerp(const float_in_vec& dt, const sse_vector& v1, const sse_vector& v2)
{
    return (v1 + ((v2 - v1) * dt));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector
sse_vector::lerp(float dt, const sse_vector& v1, const sse_vector& v2)
{
    return lerp(float_in_vec(dt), v1, v2);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::forward_vec(const sse_vector& normal,
    const sse_vector& incident,
    const sse_vector& reference)
{
    __m128 vTemp0 = reference.dotproduct(incident);
    __m128 vTemp1 = _mm_cmplt_ps(vTemp0, g_fZero);
    __m128 vTemp2 = _mm_cmpgt_ps(vTemp0, g_fZero);
    vTemp1 = _mm_and_ps(vTemp1, g_fNegativeOne);
    vTemp2 = _mm_and_ps(vTemp2, g_fOne);
    vTemp1 = _mm_or_ps(vTemp1, vTemp2); //!< Get sign
    vTemp1 = _mm_mul_ps(vTemp1, g_fNegativeOne);
    vTemp1 = _mm_mul_ps(normal.v, vTemp1);
    return vTemp1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::relect(const sse_vector& normal, const sse_vector& incident)
{
    __m128 vTemp0 = normal.dotproduct(incident);
    __m128 vTemp1 = _mm_mul_ps(normal.v, vTemp0);
    vTemp1 = _mm_mul_ps(vTemp1, g_fTwo);
    vTemp1 = _mm_sub_ps(incident.v, vTemp1);
    return vTemp1;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::operator + (const sse_vector& b) const
{
    __m128 vResult;
    vResult = _mm_add_ps(v, b.v);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::operator - (const sse_vector& b) const
{
    __m128 vResult;
    vResult = _mm_sub_ps(v, b.v);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::operator * (const sse_vector& b) const
{
    __m128 vResult;
    vResult = _mm_mul_ps(v, b.v);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::operator * (float f) const
{
    __m128 vResult;
    __m128 vSecond = _mm_set_ps1(f);
    vResult = _mm_mul_ps(v, vSecond);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline const sse_vector XR_VECTORIZED_CALL
sse_vector::operator / (const sse_vector& b) const
{
    __m128 vResult;
    vResult = _mm_div_ps(v, b.v);
    return vResult;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::equal_all(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpeq_ps(a.v, b.v);
    return _mm_movemask_ps(vTemp) == 0xF;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::equal_any(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpeq_ps(a.v, b.v);
    return _mm_movemask_ps(vTemp) != 0x0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::near_equal(const sse_vector& a, const sse_vector& b, const float epsilon)
{
    return near_equal(a, b, float_in_vec(epsilon));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::near_equal(const sse_vector& a, const sse_vector& b, const float_in_vec& epsilon)
{
    __m128 vDelta = _mm_sub_ps(a.v, b.v);
    __m128 vTemp = _mm_setzero_ps();
    vTemp = _mm_sub_ps(vTemp, vDelta);
    vTemp = _mm_max_ps(vTemp, vDelta);
    vTemp = _mm_cmple_ps(vTemp, epsilon);
    return _mm_movemask_ps(vTemp) == 0xF;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::less_all(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpge_ps(a.v, b.v);
    return _mm_movemask_ps(vTemp) == 0x0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::less_any(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpge_ps(a.v, b.v);
    return _mm_movemask_ps(vTemp) != 0xF;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::equal_all_v3(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpeq_ps(a.v, b.v);
    return (_mm_movemask_ps(vTemp) & 0x7) == 0x7;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::equal_any_v3(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpeq_ps(a.v, b.v);
    return (_mm_movemask_ps(vTemp) & 0x7) != 0x0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::near_equal_v3(const sse_vector& a, const sse_vector& b, const float epsilon)
{
    return near_equal_v3(a, b, float_in_vec(epsilon));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::near_equal_v3(const sse_vector& a, const sse_vector& b, const float_in_vec& epsilon)
{
    __m128 vDelta = _mm_sub_ps(a.v, b.v);
    __m128 vTemp = _mm_setzero_ps();
    vTemp = _mm_sub_ps(vTemp, vDelta);
    vTemp = _mm_max_ps(vTemp, vDelta);
    vTemp = _mm_cmple_ps(vTemp, epsilon);
    return _mm_movemask_ps(vTemp) == 0x7;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::less_all_v3(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpge_ps(a.v, b.v);
    return (_mm_movemask_ps(vTemp) & 0x7) == 0;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline bool XR_VECTORIZED_CALL
sse_vector::less_any_v3(const sse_vector& a, const sse_vector& b)
{
    __m128 vTemp = _mm_cmpge_ps(a.v, b.v);
    return (_mm_movemask_ps(vTemp) & 0x7) != 7;
}

} // namespace xr::math::details
//-----------------------------------------------------------------------------------------------------------
