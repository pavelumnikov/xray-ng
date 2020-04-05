#include "corlib/math/compressed_normal.h"
#include "compressed_normal_table.h"

namespace xr::math
{

// upper 3 bits
constexpr uint16_t pv_sign_mast = 0xe000;
constexpr uint16_t pv_x_sign_mask = 0x8000;
constexpr uint16_t pv_y_sign_mask = 0x4000;
constexpr uint16_t pv_z_sign_mask = 0x2000;
// middle 6 bits - xbits
constexpr uint16_t pv_top_mask = 0x1f80;
// lower 7 bits - ybits
constexpr uint16_t pv_bottom_mask = 0x007f;

uint16_t pv_compress(const vector& vec)
{
    /*
    // save copy
    vector tmp { vec };

    // input vector3 does not have to be unit length
    uint16_t mVec = 0;

    if(negative(tmp.get_x()))
        mVec |= pv_x_sign_mask; set_positive(tmp.x);
    if(negative(tmp.get_y()))
        mVec |= pv_y_sign_mask; set_positive(tmp.y);
    if(negative(tmp.z))
        mVec |= pv_z_sign_mask; set_positive(tmp.z);

    // project the normal onto the plane that goes through
    // X0=(1,0,0),Y0=(0,1,0),Z0=(0,0,1).

    // on that plane we choose an (projective!) coordinate system
    // such that X0->(0,0), Y0->(126,0), Z0->(0,126),(0,0,0)->Infinity

    // a little slower... old pack was 4 multiplies and 2 adds. 
    // This is 2 multiplies, 2 adds, and a divide....
    float w = 126.0f / (tmp.x + tmp.y + tmp.z);
    int32_t xbits = ifloor(tmp.x * w);
    int32_t ybits = ifloor(tmp.y * w);

    // Now we can be sure that 0<=xp<=126, 0<=yp<=126, 0<=xp+yp<=126

    // however for the sampling we want to transform this triangle 
    // into a rectangle.
    if(xbits >= 64)
    {
        xbits = 127 - xbits;
        ybits = 127 - ybits;
    }

    // now we that have xp in the range (0,127) and yp in the range (0,63), 
    // we can pack all the bits together
    mVec |= (xbits << 7);
    mVec |= ybits;

    return mVec;*/
}

void pv_decompress(vector& vec, uint16_t mVec)
{
    /*
    // if we do a straightforward backward transform
    // we will get points on the plane X0,Y0,Z0
    // however we need points on a sphere that goes through these points.
    // therefore we need to adjust x,y,z so that x^2+y^2+z^2=1

    // by normalizing the vector3. We have already precalculated the amount
    // by which we need to scale, so all we do is a table lookup and a 
    // multiplication

    // get the x and y bits
    int32_t xbits = ((mVec & pv_top_mask) >> 7);
    int32_t ybits = (mVec & pv_bottom_mask);

    // map the numbers back to the triangle (0,0)-(0,126)-(126,0)
    if((xbits + ybits) >= 127)
    {
        xbits = 127 - xbits;
        ybits = 127 - ybits;
    }

    // do the inverse transform and normalization
    // costs 3 extra multiplies and 2 subtracts. No big deal.         
    float uvadj = pv_uv_adj[mVec & ~pv_sign_mast];
    vec.x = uvadj * float(xbits);
    vec.y = uvadj * float(ybits);
    vec.z = uvadj * float(126 - xbits - ybits);

    // set all the sign bits
    if(mVec & pv_x_sign_mask) set_negative(vec.x);
    if(mVec & pv_y_sign_mask) set_negative(vec.y);
    if(mVec & pv_z_sign_mask) set_negative(vec.z);*/
}

} // namespace xr::math
