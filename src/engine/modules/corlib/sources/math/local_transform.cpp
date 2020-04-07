#include "corlib/math/local_transform.h"

//------------------------------------------------------------------------------
namespace xr::math
{

//------------------------------------------------------------------------------
/**
*/
matrix4 local_rigid_transform::to_matrix() const
{
    return matrix4 { pos, rot };
}

//------------------------------------------------------------------------------
/**
*/
local_rigid_transform
local_rigid_transform::operator*(local_rigid_transform const& rhs) const
{
    return
    { 
        rot.rotate(rhs.pos) + pos,
        rot * rhs.rot
    };
}

//------------------------------------------------------------------------------
/**
*/
local_rigid_transform
local_rigid_transform::interpolate(local_rigid_transform const& rhs, float t) const
{
    local_rigid_transform ret {};
    ret.pos = lerp(pos, rhs.pos, t);
    ret.rot = nlerp(rot, rhs.rot, t);
    return ret;
}

} // namespace xr::math
//------------------------------------------------------------------------------