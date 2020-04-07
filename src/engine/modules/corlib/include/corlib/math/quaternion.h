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

#include "corlib/math/vector.h"

//------------------------------------------------------------------------------
namespace xr::math
{

//------------------------------------------------------------------------------
struct quaternion
{
    constexpr quaternion() = default;

    quaternion(const vec3f& axis, float angle);
    quaternion(float _x, float _y, float _z, float _w);

    vec3f to_euler() const;
    void set(float _x, float _y, float _z, float _w);

    void conjugate();
    quaternion conjugated() const;
    void normalize();
    quaternion normalized() const;

    vec3f rotate(const vec3f& v) const;
    vec3d rotate(const vec3d& v) const;

    quaternion operator*(const quaternion& q) const;
    quaternion operator-() const;
    quaternion operator+(const quaternion& q) const;
    quaternion operator*(float m) const;
    vec3f operator*(const vec3f& q) const;

    static quaternion from_euler(const vec3f& euler);
    static quaternion vec3_to_vec3(const vec3f& a, const vec3f& b);

    float x {}, y {}, z {}, w {};

    static const quaternion IDENTITY;
}; // struct quaternion

//------------------------------------------------------------------------------
/**
*/
quaternion nlerp(const quaternion& q1, const quaternion& q2, float t);

//------------------------------------------------------------------------------
/**
*/
inline
quaternion::quaternion(float _x, float _y, float _z, float _w)
{
    x = _x; y = _y; z = _z; w = _w;
}

//------------------------------------------------------------------------------
/**
*/
inline void
quaternion::set(float _x, float _y, float _z, float _w)
{
    x = _x; y = _y; z = _z; w = _w;
}

//------------------------------------------------------------------------------
/**
*/
inline vec3f
quaternion::rotate(const vec3f& v) const
{
    // nVidia SDK implementation

    vec3f qvec(x, y, z);
    vec3f uv = cross_product(qvec, v);
    vec3f uuv = cross_product(qvec, uv);
    uv *= (2.0f * w);
    uuv *= 2.0f;

    return v + uv + uuv;
}

//------------------------------------------------------------------------------
/**
*/
inline vec3d
quaternion::rotate(const vec3d& v) const
{
    // nVidia SDK implementation

    const vec3d qvec(x, y, z);
    vec3d uv = cross_product(qvec, v);
    vec3d uuv = cross_product(qvec, uv);
    uv *= (2.0 * w);
    uuv *= 2.0;

    return v + uv + uuv;
}

} // namespace xr::math