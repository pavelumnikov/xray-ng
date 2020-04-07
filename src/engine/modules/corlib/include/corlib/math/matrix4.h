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

#include "corlib/math/quaternion.h"

namespace xr::math
{

struct XR_ALIGNAS(16) matrix4
{
    constexpr matrix4() = default;

    matrix4(const vec3f & x, const vec3f & y, const vec3f & z)
    {
        setXVector(x);
        setYVector(y);
        setZVector(z);
    }

    matrix4(const vec3f & pos, const quaternion & rot);

    explicit matrix4(const float* m)
    {
        m11 = m[0]; m12 = m[1]; m13 = m[2]; m14 = m[3];
        m21 = m[4]; m22 = m[5]; m23 = m[6]; m24 = m[7];
        m31 = m[8]; m32 = m[9]; m33 = m[10]; m34 = m[11];
        m41 = m[12]; m42 = m[13]; m43 = m[14]; m44 = m[15];
    }

    matrix4(
        float r11, float r12, float r13, float r14,
        float r21, float r22, float r23, float r24,
        float r31, float r32, float r33, float r34,
        float r41, float r42, float r43, float r44)
    {
        m11 = r11; m12 = r12; m13 = r13; m14 = r14;
        m21 = r21; m22 = r22; m23 = r23; m24 = r24;
        m31 = r31; m32 = r32; m33 = r33; m34 = r34;
        m41 = r41; m42 = r42; m43 = r43; m44 = r44;
    }

    void decompose(vec3f & position, quaternion & rotation, float& scale) const;

    float operator [](int index) const
    {
        return (&m11)[index];
    }
    float& operator [](int index)
    {
        return (&m11)[index];
    }

    matrix4 operator *(const matrix4 & rhs) const;

    matrix4 operator +(const matrix4 & rhs) const;

    matrix4 operator *(float rhs) const;

    vec4f operator *(const vec4f & rhs) const
    {
        return vec4f
        {
            m11 * rhs.x + m21 * rhs.y + m31 * rhs.z + m41 * rhs.w,
            m12 * rhs.x + m22 * rhs.y + m32 * rhs.z + m42 * rhs.w,
            m13 * rhs.x + m23 * rhs.y + m33 * rhs.z + m43 * rhs.w,
            m14 * rhs.x + m24 * rhs.y + m34 * rhs.z + m44 * rhs.w
        };
    }

    static matrix4 rotationX(float angle);
    static matrix4 rotationY(float angle);
    static matrix4 rotationZ(float angle);
    static matrix4 from_quaternion(quaternion const& rhs);

    vec3f getZVector() const
    {
        return vec3f(m31, m32, m33);
    }

    vec3f getYVector() const
    {
        return vec3f(m21, m22, m23);
    }

    vec3f getXVector() const
    {
        return vec3f(m11, m12, m13);
    }

    void setXVector(const vec3f & v)
    {
        m11 = v.x; m12 = v.y; m13 = v.z;
    }

    void setYVector(const vec3f & v)
    {
        m21 = v.x; m22 = v.y; m23 = v.z;
    }

    void setZVector(const vec3f & v)
    {
        m31 = v.x; m32 = v.y; m33 = v.z;
    }


    float determinant() const
    {
        return
            m14 * m23 * m32 * m41 - m13 * m24 * m32 * m41 - m14 * m22 * m33 * m41 + m12 * m24 * m33 * m41 +
            m13 * m22 * m34 * m41 - m12 * m23 * m34 * m41 - m14 * m23 * m31 * m42 + m13 * m24 * m31 * m42 +
            m14 * m21 * m33 * m42 - m11 * m24 * m33 * m42 - m13 * m21 * m34 * m42 + m11 * m23 * m34 * m42 +
            m14 * m22 * m31 * m43 - m12 * m24 * m31 * m43 - m14 * m21 * m32 * m43 + m11 * m24 * m32 * m43 +
            m12 * m21 * m34 * m43 - m11 * m22 * m34 * m43 - m13 * m22 * m31 * m44 + m12 * m23 * m31 * m44 +
            m13 * m21 * m32 * m44 - m11 * m23 * m32 * m44 - m12 * m21 * m33 * m44 + m11 * m22 * m33 * m44;
    }


    matrix4 inverted() const
    {
        matrix4 tmp = *this;
        tmp.inverse();
        return tmp;
    }

    void inverse()
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

    // orthonormal
    void fast_inverse()
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

    // orthonormal
    matrix4 fast_inverted() const
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

    void copy3x3(const matrix4 & mtx)
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

    void translate(const vec3f & t)
    {
        m41 += t.x;
        m42 += t.y;
        m43 += t.z;
    }

    void translate(float x, float y, float z)
    {
        m41 += x;
        m42 += y;
        m43 += z;
    }

    void set_translation(const vec3f & t)
    {
        m41 = t.x;
        m42 = t.y;
        m43 = t.z;
    }


    static matrix4 ortho(float left, float right, float bottom, float top, float z_near, 
        float z_far, bool is_homogenous_depth, bool reversed_z);

    static matrix4 from_perspective(float fov, float ratio, float near_plane, float far_plane,
        bool is_homogenous_depth, bool invert_z);

    static matrix4 from_euler(float yaw, float pitch, float roll);

    static matrix4 look_at(const vec3f& eye, const vec3f& at, const vec3f& up);

    void normalize_scale();


    void get_translation(vec3f & pos) const
    {
        pos.set(m41, m42, m43);
    }

    vec3f get_translation() const
    {
        return vec3f(m41, m42, m43);
    }

    quaternion get_rotation() const;
    void transpose();
    vec3f transform_point(const vec3f & pos) const;
    vec3f transform_vector(const vec3f & pos) const;
    void multiply_3x3(float scale);
    void set_identity();

    float m11 { 0.0f }, m12 { 0.0f }, m13 { 0.0f }, m14 { 0.0f };
    float m21 { 0.0f }, m22 { 0.0f }, m23 { 0.0f }, m24 { 0.0f };
    float m31 { 0.0f }, m32 { 0.0f }, m33 { 0.0f }, m34 { 0.0f };
    float m41 { 0.0f }, m42 { 0.0f }, m43 { 0.0f }, m44 { 0.0f };

    static const matrix4 IDENTITY;
};

} // namespace xr::math