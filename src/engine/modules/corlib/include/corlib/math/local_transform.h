#pragma once

#include "corlib/math/matrix4.h"

//------------------------------------------------------------------------------
namespace xr::math
{

//------------------------------------------------------------------------------
struct local_rigid_transform
{
    constexpr local_rigid_transform() noexcept = default;
    constexpr local_rigid_transform(vec3f const& position, quaternion const& rotation) noexcept;

    local_rigid_transform inverted() const noexcept;

    matrix4 to_matrix() const noexcept;
    local_rigid_transform operator*(local_rigid_transform const& rhs) const noexcept;
    local_rigid_transform interpolate(local_rigid_transform const& rhs, float t) const noexcept;

    vec3f pos {};
    quaternion rot {};
}; // struct local_rigid_transform

//------------------------------------------------------------------------------
/**
*/
constexpr
local_rigid_transform::local_rigid_transform(vec3f const& position, quaternion const& rotation) noexcept
    : pos { position }
    , rot { rotation }
{}

//------------------------------------------------------------------------------
/**
*/
local_rigid_transform local_rigid_transform::inverted() const noexcept
{
    local_rigid_transform result {};
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos);
    return result;
}

//------------------------------------------------------------------------------
struct rigid_transform
{
    constexpr rigid_transform() noexcept = default;
    constexpr rigid_transform(vec3d const& pos, quaternion const& rot) noexcept;

    rigid_transform inverted() const noexcept;

    rigid_transform operator*(rigid_transform const& rhs) const noexcept;
    rigid_transform operator*(local_rigid_transform const& rhs) const noexcept;

    quaternion rot {};
    vec3d pos {};
}; // struct rigid_transform

//------------------------------------------------------------------------------
/**
*/
constexpr rigid_transform::rigid_transform(const vec3d& pos, const quaternion& rot) noexcept
    : pos(pos)
    , rot(rot)
{}

//------------------------------------------------------------------------------
/**
*/
inline rigid_transform rigid_transform::inverted() const noexcept
{
    rigid_transform result;
    result.rot = rot.conjugated();
    result.pos = result.rot.rotate(-pos);
    return result;
}

//------------------------------------------------------------------------------
/**
*/
inline rigid_transform rigid_transform::operator*(const rigid_transform& rhs) const noexcept
{
    return{ rot.rotate(rhs.pos) + pos, rot * rhs.rot };
}

//------------------------------------------------------------------------------
/**
*/
rigid_transform rigid_transform::operator*(const local_rigid_transform& rhs) const noexcept
{
    return { vec3d(rot.rotate(rhs.pos)) + pos, rot * rhs.rot };
}


//------------------------------------------------------------------------------
struct transform
{
    constexpr transform() noexcept = default;
    constexpr transform(vec3d const& _pos, quaternion const& _rot, float _scale) noexcept;

    transform inverted() const noexcept;

    constexpr transform operator*(transform const& rhs) const noexcept;
    constexpr transform operator*(local_rigid_transform const& rhs) const noexcept;

    constexpr vec3d tform(vec3f const& value) const noexcept;
    constexpr vec3d tform(vec3d const& value) const noexcept;
    constexpr rigid_transform get_rigid_path() const noexcept;

    vec3d pos {};
    quaternion rot {};
    float scale {};

    static const transform IDENTITY;
};

//------------------------------------------------------------------------------
/**
*/
constexpr transform::transform(vec3d const& _pos, quaternion const& _rot, float _scale) noexcept
    : pos { _pos }
    , rot { _rot }
    , scale { _scale }
{}

//------------------------------------------------------------------------------
/**
*/
inline transform transform::inverted() const noexcept
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
constexpr transform transform::operator*(transform const& rhs) const noexcept
{
    return { rot.rotate(rhs.pos * scale) + pos, rot * rhs.rot, scale * rhs.scale };
}

//------------------------------------------------------------------------------
/**
*/
constexpr transform transform::operator*(local_rigid_transform const& rhs) const noexcept
{
    return { pos + rot.rotate(rhs.pos * scale), rot * rhs.rot, scale };
}

//------------------------------------------------------------------------------
/**
*/
constexpr vec3d transform::tform(vec3f const& value) const noexcept
{
    return vec3d { pos + rot.rotate(value) * scale };
}

//------------------------------------------------------------------------------
/**
*/
constexpr vec3d transform::tform(const vec3d& value) const noexcept
{
    return vec3d { pos + rot.rotate(value) * scale };
}

//------------------------------------------------------------------------------
/**
*/
constexpr rigid_transform transform::get_rigid_path() const noexcept
{
    return { pos, rot };
}

} // namespace xr::math
//------------------------------------------------------------------------------