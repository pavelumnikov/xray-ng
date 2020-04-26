// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/matrix4.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

//-----------------------------------------------------------------------------------------------------------
struct local_rigid_transform
{
    XR_CONSTEXPR_CPP14_OR_INLINE local_rigid_transform() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE local_rigid_transform(vec3f const& position, quaternion const& rotation) XR_NOEXCEPT;

    local_rigid_transform inverted() const XR_NOEXCEPT;

    matrix4 to_matrix() const XR_NOEXCEPT;
    local_rigid_transform operator*(local_rigid_transform const& rhs) const XR_NOEXCEPT;
    local_rigid_transform interpolate(local_rigid_transform const& rhs, float t) const XR_NOEXCEPT;

    vec3f pos {};
    quaternion rot {};
}; // struct local_rigid_transform

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE
local_rigid_transform::local_rigid_transform() XR_NOEXCEPT
    : pos()
    , rot()
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE
local_rigid_transform::local_rigid_transform(vec3f const& position, quaternion const& rotation) XR_NOEXCEPT
    : pos(position)
    , rot(rotation)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
local_rigid_transform local_rigid_transform::inverted() const XR_NOEXCEPT
{
    local_rigid_transform result;
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
struct rigid_transform
{
    XR_CONSTEXPR_CPP14_OR_INLINE rigid_transform() XR_NOEXCEPT = default;
    XR_CONSTEXPR_CPP14_OR_INLINE rigid_transform(vec3d const& pos, quaternion const& rot) XR_NOEXCEPT;

    rigid_transform inverted() const XR_NOEXCEPT;

    rigid_transform operator*(rigid_transform const& rhs) const XR_NOEXCEPT;
    rigid_transform operator*(local_rigid_transform const& rhs) const XR_NOEXCEPT;

    quaternion rot {};
    vec3d pos {};
}; // struct rigid_transform

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE rigid_transform::rigid_transform(const vec3d& pos, const quaternion& rot) XR_NOEXCEPT
    : pos(pos)
    , rot(rot)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline rigid_transform rigid_transform::inverted() const XR_NOEXCEPT
{
    rigid_transform result;
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos);
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline rigid_transform rigid_transform::operator*(const rigid_transform& rhs) const XR_NOEXCEPT
{
    return{ rot.rotate(rhs.pos) + pos, rot * rhs.rot };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
rigid_transform rigid_transform::operator*(const local_rigid_transform& rhs) const XR_NOEXCEPT
{
    return { vec3d(rot.rotate(rhs.pos)) + pos, rot * rhs.rot };
}


//-----------------------------------------------------------------------------------------------------------
struct transform
{
    XR_CONSTEXPR_CPP14_OR_INLINE transform() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE transform(vec3d const& _pos, quaternion const& _rot, float _scale) XR_NOEXCEPT;

    transform inverted() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE transform operator*(transform const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE transform operator*(local_rigid_transform const& rhs) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3d tform(vec3f const& value) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d tform(vec3d const& value) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE rigid_transform get_rigid_path() const XR_NOEXCEPT;

    vec3d pos;
    quaternion rot;
    float scale;

    static const transform IDENTITY;
}; // struct transform

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE transform::transform() XR_NOEXCEPT
    : pos()
    , rot()
    , scale(1.0f)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE transform::transform(vec3d const& _pos, quaternion const& _rot, float _scale) XR_NOEXCEPT
    : pos(_pos)
    , rot(_rot)
    , scale(_scale)
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
inline transform transform::inverted() const XR_NOEXCEPT
{
    transform result {};
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos / scale);
    result.scale = 1.0f / scale;
    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE transform transform::operator*(transform const& rhs) const XR_NOEXCEPT
{
    return transform(rot.rotate(rhs.pos * scale) + pos, rot * rhs.rot, scale * rhs.scale);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE transform transform::operator*(local_rigid_transform const& rhs) const XR_NOEXCEPT
{
    return transform(pos + rot.rotate(rhs.pos * scale), rot * rhs.rot, scale);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE vec3d transform::tform(vec3f const& value) const XR_NOEXCEPT
{
    return vec3d(pos + rot.rotate(value) * scale);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE vec3d transform::tform(const vec3d& value) const XR_NOEXCEPT
{
    return vec3d(pos + rot.rotate(value) * scale);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE rigid_transform transform::get_rigid_path() const XR_NOEXCEPT
{
    return rigid_transform(pos, rot);
}

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
