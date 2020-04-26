// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/vector.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

//-----------------------------------------------------------------------------------------------------------
struct quaternion
{
    XR_CONSTEXPR_CPP14_OR_INLINE quaternion() XR_NOEXCEPT;

    quaternion(vec3f const& axis, float angle) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE quaternion(float x, float y, float z, float w) XR_NOEXCEPT;

    vec3f to_euler() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void set(float x, float y, float z, float w) XR_NOEXCEPT;

    void conjugate() XR_NOEXCEPT;
    quaternion conjugated() const XR_NOEXCEPT;
    void normalize() XR_NOEXCEPT;
    quaternion normalized() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3f rotate(vec3f const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d rotate(vec3d const& v) const XR_NOEXCEPT;

    quaternion operator*(const quaternion& q) const XR_NOEXCEPT;
    quaternion operator-() const XR_NOEXCEPT;
    quaternion operator+(const quaternion& q) const XR_NOEXCEPT;
    quaternion operator*(float m) const XR_NOEXCEPT;
    vec3f operator*(vec3f const& q) const XR_NOEXCEPT;

    static quaternion from_euler(vec3f const& euler) XR_NOEXCEPT;
    static quaternion vec3_to_vec3(vec3f const& a, vec3f const& b) XR_NOEXCEPT;

    float x, y, z, w;
    static const quaternion IDENTITY;
}; // struct quaternion

//-----------------------------------------------------------------------------------------------------------
/**
 */
quaternion nlerp(const quaternion& q1, const quaternion& q2, float t) XR_NOEXCEPT;

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE
quaternion::quaternion() XR_NOEXCEPT
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , w(0.0f)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE
quaternion::quaternion(float x, float y, float z, float w) XR_NOEXCEPT
    : x(x)
    , y(y)
    , z(z)
    , w(w)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void
quaternion::set(float x, float y, float z, float w) XR_NOEXCEPT
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f
quaternion::rotate(vec3f const& v) const XR_NOEXCEPT
{
    // nVidia SDK implementation

    vec3f qvec(x, y, z);
    vec3f uv = cross_product(qvec, v);
    vec3f uuv = cross_product(qvec, uv);
    uv *= (2.0f * w);
    uuv *= 2.0f;

    return v + uv + uuv;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d
quaternion::rotate(vec3d const& v) const XR_NOEXCEPT
{
    // nVidia SDK implementation

    const vec3d qvec(x, y, z);
    vec3d uv = cross_product(qvec, v);
    vec3d uuv = cross_product(qvec, uv);
    uv *= (2.0 * w);
    uuv *= 2.0;

    return v + uv + uuv;
}

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
