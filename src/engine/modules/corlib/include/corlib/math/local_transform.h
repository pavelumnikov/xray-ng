#pragma once

#include "corlib/math/matrix4.h"

//------------------------------------------------------------------------------
namespace xr::math
{

//------------------------------------------------------------------------------
struct local_rigid_transform
{
    local_rigid_transform() = default;
    local_rigid_transform(vec3f const& position, quaternion const& rotation);

    local_rigid_transform inverted() const;

    matrix4 to_matrix() const;
    local_rigid_transform operator*(local_rigid_transform const& rhs) const;
    local_rigid_transform interpolate(local_rigid_transform const& rhs, float t) const;

    vec3f pos {};
    quaternion rot {};
}; // struct local_rigid_transform

//------------------------------------------------------------------------------
/**
*/
inline
local_rigid_transform::local_rigid_transform(vec3f const& position, quaternion const& rotation)
    : pos { position }
    , rot { rotation }
{}

//------------------------------------------------------------------------------
/**
*/
local_rigid_transform local_rigid_transform::inverted() const
{
    local_rigid_transform result {};
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos);
    return result;
}

//------------------------------------------------------------------------------
struct rigid_transform
{
    rigid_transform() = default;
    rigid_transform(vec3d const& pos, quaternion const& rot);

    rigid_transform inverted() const;

    rigid_transform operator*(rigid_transform const& rhs) const;
    rigid_transform operator*(local_rigid_transform const& rhs) const;

    quaternion rot {};
    vec3d pos {};
}; // struct rigid_transform

//------------------------------------------------------------------------------
/**
*/
inline rigid_transform::rigid_transform(const vec3d& pos, const quaternion& rot)
    : pos(pos)
    , rot(rot)
{}

//------------------------------------------------------------------------------
/**
*/
inline rigid_transform rigid_transform::inverted() const
{
    rigid_transform result;
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
inline rigid_transform rigid_transform::operator*(const rigid_transform& rhs) const
{
    return{ rot.rotate(rhs.pos) + pos, rot * rhs.rot };
}

//------------------------------------------------------------------------------
/**
*/
rigid_transform rigid_transform::operator*(const local_rigid_transform& rhs) const
{
    return { vec3d(rot.rotate(rhs.pos)) + pos, rot * rhs.rot };
}


//------------------------------------------------------------------------------
struct transform
{
    constexpr transform() = default;
    transform(vec3d const& _pos, quaternion const& _rot, float _scale);

    transform inverted() const;

    transform operator*(transform const& rhs) const;
    transform operator*(local_rigid_transform const& rhs) const;

    vec3d tform(vec3f const& value) const;
    vec3d tform(vec3d const& value) const;
    rigid_transform get_rigid_path() const;

    vec3d pos {};
    quaternion rot {};
    float scale {};

    static const transform IDENTITY;
};

//------------------------------------------------------------------------------
/**
*/
inline transform::transform(vec3d const& _pos, quaternion const& _rot, float _scale)
    : pos { _pos }
    , rot { _rot }
    , scale { _scale }
{}

//------------------------------------------------------------------------------
/**
*/
inline transform transform::inverted() const
{
    transform result {};
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos / scale);
    result.scale = 1.0f / scale;
    return result;
}

//------------------------------------------------------------------------------
/**
*/
transform transform::operator*(transform const& rhs) const
{
    return { rot.rotate(rhs.pos * scale) + pos, rot * rhs.rot, scale * rhs.scale };
}

//------------------------------------------------------------------------------
/**
*/
inline transform transform::operator*(local_rigid_transform const& rhs) const
{
    return { pos + rot.rotate(rhs.pos * scale), rot * rhs.rot, scale };
}

//------------------------------------------------------------------------------
/**
*/
inline vec3d transform::tform(vec3f const& value) const
{
    return vec3d { pos + rot.rotate(value) * scale };
}

//------------------------------------------------------------------------------
/**
*/
inline vec3d transform::tform(const vec3d& value) const
{
    return vec3d { pos + rot.rotate(value) * scale };
}

//------------------------------------------------------------------------------
/**
*/
inline rigid_transform transform::get_rigid_path() const
{
    return { pos, rot };
}

} // namespace xr::math
//------------------------------------------------------------------------------