// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/quaternion.h"

//------------------------------------------------------------------------------
namespace xr::math
{

//------------------------------------------------------------------------------
struct XR_ALIGNAS(16) matrix4
{
    constexpr matrix4() noexcept = default;
    constexpr matrix4(vec3f const& x, vec3f const& y, vec3f const& z) noexcept;
    matrix4(vec3f const& pos, const quaternion & rot) noexcept;
    explicit matrix4(const float* m) noexcept;

    constexpr matrix4(
        float r11, float r12, float r13, float r14,
        float r21, float r22, float r23, float r24,
        float r31, float r32, float r33, float r34,
        float r41, float r42, float r43, float r44) noexcept;

    void decompose(vec3f & position, quaternion & rotation, float& scale) const noexcept;

    float operator [](int index) const noexcept;
    float& operator [](int index) noexcept;

    constexpr vec4f operator *(vec4f const& rhs) const noexcept;

    matrix4 operator *(float rhs) const noexcept;
    matrix4 operator *(matrix4 const& rhs) const noexcept;
    matrix4 operator +(matrix4 const& rhs) const noexcept;

    static matrix4 rotationX(float angle) noexcept;
    static matrix4 rotationY(float angle) noexcept;
    static matrix4 rotationZ(float angle) noexcept;
    static matrix4 from_quaternion(quaternion const& rhs) noexcept;

    constexpr vec3f getXVector() const noexcept;
    constexpr vec3f getYVector() const noexcept;
    constexpr vec3f getZVector() const noexcept;

    constexpr void set_xvec(vec3f const& v) noexcept;
    constexpr void set_yvec(vec3f const& v) noexcept;
    constexpr void set_zvec(vec3f const& v) noexcept;

    constexpr float determinant() const noexcept;

    matrix4 inverted() const noexcept;
    constexpr void inverse() noexcept;

    // orthonormal
    constexpr void fast_inverse() noexcept;

    // orthonormal
    constexpr matrix4 fast_inverted() const noexcept;

    constexpr void copy3x3(matrix4 const& mtx) noexcept;

    constexpr void translate(vec3f const& t) noexcept;
    constexpr void translate(float x, float y, float z) noexcept;

    static matrix4 ortho(float left, float right, float bottom, float top, float z_near, 
        float z_far, bool is_homogenous_depth, bool reversed_z) noexcept;

    static matrix4 from_perspective(float fov, float ratio, float near_plane, float far_plane,
        bool is_homogenous_depth, bool invert_z) noexcept;

    static matrix4 from_euler(float yaw, float pitch, float roll) noexcept;

    static matrix4 look_at(vec3f const& eye, vec3f const& at, vec3f const& up) noexcept;

    void normalize_scale() noexcept;

    constexpr void set_translation(vec3f const& t) noexcept;
    constexpr void get_translation(vec3f& pos) const noexcept;
    constexpr vec3f get_translation() const noexcept;
    quaternion get_rotation() const noexcept;

    void transpose() noexcept;
    vec3f transform_point(vec3f const& pos) const noexcept;
    vec3f transform_vector(vec3f const& pos) const noexcept;

    void multiply_3x3(float scale) noexcept;
    void set_identity() noexcept;

    float m11 { 0.0f }, m12 { 0.0f }, m13 { 0.0f }, m14 { 0.0f };
    float m21 { 0.0f }, m22 { 0.0f }, m23 { 0.0f }, m24 { 0.0f };
    float m31 { 0.0f }, m32 { 0.0f }, m33 { 0.0f }, m34 { 0.0f };
    float m41 { 0.0f }, m42 { 0.0f }, m43 { 0.0f }, m44 { 0.0f };

    static const matrix4 IDENTITY;
};

//------------------------------------------------------------------------------
/**
 */
constexpr matrix4::matrix4(vec3f const& x, vec3f const& y, vec3f const& z) noexcept
{
    set_xvec(x);
    set_yvec(y);
    set_zvec(z);
}

//------------------------------------------------------------------------------
/**
 */
inline matrix4::matrix4(float const* m) noexcept
{
    m11 = m[0]; m12 = m[1]; m13 = m[2]; m14 = m[3];
    m21 = m[4]; m22 = m[5]; m23 = m[6]; m24 = m[7];
    m31 = m[8]; m32 = m[9]; m33 = m[10]; m34 = m[11];
    m41 = m[12]; m42 = m[13]; m43 = m[14]; m44 = m[15];
}

//------------------------------------------------------------------------------
/**
 */
constexpr matrix4::matrix4(
    float r11, float r12, float r13, float r14,
    float r21, float r22, float r23, float r24,
    float r31, float r32, float r33, float r34,
    float r41, float r42, float r43, float r44) noexcept
{
    m11 = r11; m12 = r12; m13 = r13; m14 = r14;
    m21 = r21; m22 = r22; m23 = r23; m24 = r24;
    m31 = r31; m32 = r32; m33 = r33; m34 = r34;
    m41 = r41; m42 = r42; m43 = r43; m44 = r44;
}

//------------------------------------------------------------------------------
/**
 */
inline float matrix4::operator [](int index) const noexcept
{
    return (&m11)[index];
}

//------------------------------------------------------------------------------
/**
 */
inline float& matrix4::operator [](int index) noexcept
{
    return (&m11)[index];
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f matrix4::operator *(const vec4f& rhs) const noexcept
{
    return vec4f
    {
        m11 * rhs.x + m21 * rhs.y + m31 * rhs.z + m41 * rhs.w,
        m12 * rhs.x + m22 * rhs.y + m32 * rhs.z + m42 * rhs.w,
        m13 * rhs.x + m23 * rhs.y + m33 * rhs.z + m43 * rhs.w,
        m14 * rhs.x + m24 * rhs.y + m34 * rhs.z + m44 * rhs.w
    };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f matrix4::getZVector() const noexcept
{
    return vec3f { m31, m32, m33 };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f matrix4::getYVector() const noexcept
{
    return vec3f { m21, m22, m23 };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f matrix4::getXVector() const noexcept
{
    return vec3f { m11, m12, m13 };
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::set_xvec(vec3f const& v) noexcept
{
    m11 = v.x; m12 = v.y; m13 = v.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::set_yvec(vec3f const& v) noexcept
{
    m21 = v.x; m22 = v.y; m23 = v.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::set_zvec(vec3f const& v) noexcept
{
    m31 = v.x; m32 = v.y; m33 = v.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr float matrix4::determinant() const noexcept
{
    return
        m14 * m23 * m32 * m41 - m13 * m24 * m32 * m41 - m14 * m22 * m33 * m41 + m12 * m24 * m33 * m41 +
        m13 * m22 * m34 * m41 - m12 * m23 * m34 * m41 - m14 * m23 * m31 * m42 + m13 * m24 * m31 * m42 +
        m14 * m21 * m33 * m42 - m11 * m24 * m33 * m42 - m13 * m21 * m34 * m42 + m11 * m23 * m34 * m42 +
        m14 * m22 * m31 * m43 - m12 * m24 * m31 * m43 - m14 * m21 * m32 * m43 + m11 * m24 * m32 * m43 +
        m12 * m21 * m34 * m43 - m11 * m22 * m34 * m43 - m13 * m22 * m31 * m44 + m12 * m23 * m31 * m44 +
        m13 * m21 * m32 * m44 - m11 * m23 * m32 * m44 - m12 * m21 * m33 * m44 + m11 * m22 * m33 * m44;
}

//------------------------------------------------------------------------------
/**
 */
inline matrix4 matrix4::inverted() const noexcept
{
    matrix4 tmp = *this;
    tmp.inverse();
    return tmp;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::inverse() noexcept
{
    matrix4 mtx;
    float d = determinant();
    if(d == 0) return;
    d = 1.0f / d;

    mtx.m11 = d * (m23 * m34 * m42 - m24 * m33 * m42 + m24 * m32 * m43 - m22 * m34 * m43 - m23 * m32 * m44 + m22 * m33 * m44);
    mtx.m12 = d * (m14 * m33 * m42 - m13 * m34 * m42 - m14 * m32 * m43 + m12 * m34 * m43 + m13 * m32 * m44 - m12 * m33 * m44);
    mtx.m13 = d * (m13 * m24 * m42 - m14 * m23 * m42 + m14 * m22 * m43 - m12 * m24 * m43 - m13 * m22 * m44 + m12 * m23 * m44);
    mtx.m14 = d * (m14 * m23 * m32 - m13 * m24 * m32 - m14 * m22 * m33 + m12 * m24 * m33 + m13 * m22 * m34 - m12 * m23 * m34);
    mtx.m21 = d * (m24 * m33 * m41 - m23 * m34 * m41 - m24 * m31 * m43 + m21 * m34 * m43 + m23 * m31 * m44 - m21 * m33 * m44);
    mtx.m22 = d * (m13 * m34 * m41 - m14 * m33 * m41 + m14 * m31 * m43 - m11 * m34 * m43 - m13 * m31 * m44 + m11 * m33 * m44);
    mtx.m23 = d * (m14 * m23 * m41 - m13 * m24 * m41 - m14 * m21 * m43 + m11 * m24 * m43 + m13 * m21 * m44 - m11 * m23 * m44);
    mtx.m24 = d * (m13 * m24 * m31 - m14 * m23 * m31 + m14 * m21 * m33 - m11 * m24 * m33 - m13 * m21 * m34 + m11 * m23 * m34);
    mtx.m31 = d * (m22 * m34 * m41 - m24 * m32 * m41 + m24 * m31 * m42 - m21 * m34 * m42 - m22 * m31 * m44 + m21 * m32 * m44);
    mtx.m32 = d * (m14 * m32 * m41 - m12 * m34 * m41 - m14 * m31 * m42 + m11 * m34 * m42 + m12 * m31 * m44 - m11 * m32 * m44);
    mtx.m33 = d * (m12 * m24 * m41 - m14 * m22 * m41 + m14 * m21 * m42 - m11 * m24 * m42 - m12 * m21 * m44 + m11 * m22 * m44);
    mtx.m34 = d * (m14 * m22 * m31 - m12 * m24 * m31 - m14 * m21 * m32 + m11 * m24 * m32 + m12 * m21 * m34 - m11 * m22 * m34);
    mtx.m41 = d * (m23 * m32 * m41 - m22 * m33 * m41 - m23 * m31 * m42 + m21 * m33 * m42 + m22 * m31 * m43 - m21 * m32 * m43);
    mtx.m42 = d * (m12 * m33 * m41 - m13 * m32 * m41 + m13 * m31 * m42 - m11 * m33 * m42 - m12 * m31 * m43 + m11 * m32 * m43);
    mtx.m43 = d * (m13 * m22 * m41 - m12 * m23 * m41 - m13 * m21 * m42 + m11 * m23 * m42 + m12 * m21 * m43 - m11 * m22 * m43);
    mtx.m44 = d * (m12 * m23 * m31 - m13 * m22 * m31 + m13 * m21 * m32 - m11 * m23 * m32 - m12 * m21 * m33 + m11 * m22 * m33);

    *this = mtx;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::fast_inverse() noexcept
{
    float tmp = m21;
    m21 = m12;
    m12 = tmp;

    tmp = m32;
    m32 = m23;
    m23 = tmp;

    tmp = m31;
    m31 = m13;
    m13 = tmp;

    float m41 = -this->m41;
    float m42 = -this->m42;
    float m43 = -this->m43;
    this->m41 = m41 * m11 + m42 * m21 + m43 * m31;
    this->m42 = m41 * m12 + m42 * m22 + m43 * m32;
    this->m43 = m41 * m13 + m42 * m23 + m43 * m33;
}

//------------------------------------------------------------------------------
/**
 */
constexpr matrix4 matrix4::fast_inverted() const noexcept
{
    matrix4 ret {};

    ret.m11 = m11;
    ret.m22 = m22;
    ret.m33 = m33;

    ret.m21 = m12;
    ret.m12 = m12;

    ret.m23 = m32;
    ret.m32 = m23;

    ret.m13 = m31;
    ret.m31 = m13;

    ret.m41 = -m41;
    ret.m42 = -m42;
    ret.m43 = -m43;
    ret.m41 = ret.m41 * ret.m11 + ret.m42 * ret.m21 + ret.m43 * ret.m31;
    ret.m42 = ret.m41 * ret.m12 + ret.m42 * ret.m22 + ret.m43 * ret.m32;
    ret.m43 = ret.m41 * ret.m13 + ret.m42 * ret.m23 + ret.m43 * ret.m33;

    ret.m14 = 0;
    ret.m24 = 0;
    ret.m34 = 0;
    ret.m44 = 1;

    return ret;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::copy3x3(const matrix4& mtx) noexcept
{
    m11 = mtx.m11;
    m12 = mtx.m12;
    m13 = mtx.m13;

    m21 = mtx.m21;
    m22 = mtx.m22;
    m23 = mtx.m23;

    m31 = mtx.m31;
    m32 = mtx.m32;
    m33 = mtx.m33;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::translate(vec3f const& t) noexcept
{
    m41 += t.x;
    m42 += t.y;
    m43 += t.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::translate(float x, float y, float z) noexcept
{
    m41 += x;
    m42 += y;
    m43 += z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::set_translation(vec3f const& t) noexcept
{
    m41 = t.x;
    m42 = t.y;
    m43 = t.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void matrix4::get_translation(vec3f& pos) const noexcept
{
    pos.set(m41, m42, m43);
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f matrix4::get_translation() const noexcept
{
    return vec3f(m41, m42, m43);
}

} // namespace xr::math