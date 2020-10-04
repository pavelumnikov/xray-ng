// This file is a part of xray-ng engine
//

#include "corlib/math/local_transform.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 local_rigid_transform::to_matrix() const XR_NOEXCEPT
{
    return matrix4(pos, rot);
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
local_rigid_transform
local_rigid_transform::operator*(local_rigid_transform const& rhs) const XR_NOEXCEPT
{ 
    return local_rigid_transform(
        rot.rotate(rhs.pos) + pos,
        rot * rhs.rot
    );
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
local_rigid_transform
local_rigid_transform::interpolate(local_rigid_transform const& rhs, float t) const XR_NOEXCEPT
{
    local_rigid_transform ret {};
    ret.pos = lerp(pos, rhs.pos, t);
    ret.rot = nlerp(rot, rhs.rot, t);
    return ret;
}

XR_NAMESPACE_END(xr, mathe)
//-----------------------------------------------------------------------------------------------------------
