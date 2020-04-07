#include "corlib/math/quaternion.h"
#include "corlib/math/mathlib.h"

namespace xr::math
{

const quaternion quaternion::IDENTITY = { 0, 0, 0, 1 };

quaternion::quaternion(vec3f const& axis, float angle)
{
    float const half_angle = angle * 0.5f;
    float const s = sine(half_angle);
    w = cosine(half_angle);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
}


quaternion quaternion::vec3_to_vec3(vec3f const& v0, vec3f const& v1)
{
    vec3f const from = v0.normalized();
    vec3f const to = v1.normalized();
    float cos_angle = dot_product(from, to);

    vec3f half;
    if(cos_angle > -1.0005f && cos_angle < -0.9995f)
        half = cross_product(from, vec3f { to.x + 0.3f, to.y - 0.15f, to.z - 0.15f }).normalized();
    else
        half = (from + to).normalized();

    // http://physicsforgames.blogspot.sk/2010/03/quaternion-tricks.html
    return quaternion
    {
        from.y * half.z - from.z * half.y,
        from.z * half.x - from.x * half.z,
        from.x * half.y - from.y * half.x,
        dot_product(from, half)
    };
}

vec3f quaternion::to_euler() const
{
    // from urho3d
    float const check = 2.0f * (-y * z + w * x);

    if(check < -0.995f)
    {
        return vec3f
        {
            -pi * 0.5f, 0.0f, -atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
        };
    }

    if(check > 0.995f)
    {
        return vec3f
        {
            pi * 0.5f, 0.0f, atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
        };
    }

    return vec3f
    {
        asin_fast4(check),
        atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)),
        atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z))
    };
}


void quaternion::conjugate()
{
    w = -w;
}


quaternion quaternion::conjugated() const
{
    return quaternion { x, y, z, -w };
}

void quaternion::normalize()
{
    float l = rsqrt(x * x + y * y + z * z + w * w);
    x *= l;
    y *= l;
    z *= l;
    w *= l;
}

quaternion quaternion::normalized() const
{
    float l = rsqrt(x * x + y * y + z * z + w * w);
    return quaternion { x * l, y * l, z * l, w * l };
}

quaternion quaternion::operator*(const quaternion& rhs) const
{
    return quaternion
    {
        w * rhs.x + rhs.w * x + y * rhs.z - rhs.y * z,
        w * rhs.y + rhs.w * y + z * rhs.x - rhs.z * x,
        w * rhs.z + rhs.w * z + x * rhs.y - rhs.x * y,
        w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
    };
}


quaternion quaternion::operator-() const
{
    return quaternion { x, y, z, -w };
}


quaternion quaternion::operator+(const quaternion& q) const
{
    return quaternion { x + q.x, y + q.y, z + q.z, w + q.w };
}


quaternion quaternion::operator*(float m) const
{
    return quaternion { x * m, y * m, z * m, w * m };
}


vec3f quaternion::operator*(vec3f const& q) const
{
    return rotate(q);
}

quaternion quaternion::from_euler(vec3f const& euler)
{
    XR_DEBUG_ASSERTION(euler.x >= -pi_div_2 && euler.x <= pi_div_2);

    float const ex = euler.x * 0.5f;
    float const ey = euler.y * 0.5f;
    float const ez = euler.z * 0.5f;
    float const sinX = sine(ex);
    float const cosX = cosine(ex);
    float const sinY = sine(ey);
    float const cosY = cosine(ey);
    float const sinZ = sine(ez);
    float const cosZ = cosine(ez);

    return quaternion
    {
        cosY * sinX * cosZ + sinY * cosX * sinZ, // x
        sinY * cosX * cosZ - cosY * sinX * sinZ, // y
        cosY * cosX * sinZ - sinY * sinX * cosZ, // z
        cosY * cosX * cosZ + sinY * sinX * sinZ  // w
    };
}

quaternion nlerp(const quaternion& q1, const quaternion& q2, float t)
{
    quaternion res {};

    float inv = 1.0f - t;
    if(q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w < 0) t = -t;

    float ox = q1.x * inv + q2.x * t;
    float oy = q1.y * inv + q2.y * t;
    float oz = q1.z * inv + q2.z * t;
    float ow = q1.w * inv + q2.w * t;
    float l = rsqrt(ox * ox + oy * oy + oz * oz + ow * ow);

    ox *= l;
    oy *= l;
    oz *= l;
    ow *= l;
    res.x = ox;
    res.y = oy;
    res.z = oz;
    res.w = ow;
    return res;
}

} // namespace xr::math