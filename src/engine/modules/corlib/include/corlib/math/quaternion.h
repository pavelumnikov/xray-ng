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
    constexpr quaternion() noexcept = default;

    quaternion(vec3f const& axis, float angle) noexcept;
    constexpr quaternion(float x, float y, float z, float w) noexcept;

    vec3f to_euler() const noexcept;
    constexpr void set(float x, float y, float z, float w) noexcept;

    void conjugate() noexcept;
    quaternion conjugated() const noexcept;
    void normalize() noexcept;
    quaternion normalized() const noexcept;

    constexpr vec3f rotate(vec3f const& v) const noexcept;
    constexpr vec3d rotate(vec3d const& v) const noexcept;

    quaternion operator*(const quaternion& q) const noexcept;
    quaternion operator-() const noexcept;
    quaternion operator+(const quaternion& q) const noexcept;
    quaternion operator*(float m) const noexcept;
    vec3f operator*(vec3f const& q) const noexcept;

    static quaternion from_euler(vec3f const& euler) noexcept;
    static quaternion vec3_to_vec3(vec3f const& a, vec3f const& b) noexcept;

    float x {}, y {}, z {}, w {};

    static const quaternion IDENTITY;
}; // struct quaternion

//------------------------------------------------------------------------------
/**
*/
quaternion nlerp(const quaternion& q1, const quaternion& q2, float t) noexcept;

//------------------------------------------------------------------------------
/**
*/
constexpr
quaternion::quaternion(float x, float y, float z, float w) noexcept
    : x { x }
    , y { y }
    , z { z }
    , w { w }
{}

//------------------------------------------------------------------------------
/**
*/
constexpr void
quaternion::set(float x, float y, float z, float w) noexcept
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//------------------------------------------------------------------------------
/**
*/
constexpr vec3f
quaternion::rotate(vec3f const& v) const noexcept
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
constexpr vec3d
quaternion::rotate(vec3d const& v) const noexcept
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
//------------------------------------------------------------------------------