// This file is a part of xray-ng engine
//

#include "corlib/math/matrix4.h"
#include "corlib/math/mathlib.h"
#include "corlib/math/details/sse/sse_math_intrinsics.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::math
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::rotationX(float angle) noexcept
{
    matrix4 m {};

    float const c = cosine(angle);
    float const s = sine(angle);

    m.m22 = m.m33 = c;
    m.m32 = -s;
    m.m23 = s;

    return m;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::rotationY(float angle) noexcept
{
    matrix4 m {};

    float const c = cosine(angle);
    float const s = sine(angle);

    m.m11 = m.m33 = c;
    m.m31 = s;
    m.m13 = -s;

    return m;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::rotationZ(float angle) noexcept
{
    matrix4 m {};

    float const c = cosine(angle);
    float const s = sine(angle);

    m.m11 = m.m22 = c;
    m.m21 = -s;
    m.m12 = s;

    return m;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::from_quaternion(quaternion const& rhs) noexcept
{
    // TODO: rewrite with SSE

    float fx = rhs.x + rhs.x;
    float fy = rhs.y + rhs.y;
    float fz = rhs.z + rhs.z;
    float fwx = fx * rhs.w;
    float fwy = fy * rhs.w;
    float fwz = fz * rhs.w;
    float fxx = fx * rhs.x;
    float fxy = fy * rhs.x;
    float fxz = fz * rhs.x;
    float fyy = fy * rhs.y;
    float fyz = fz * rhs.y;
    float fzz = fz * rhs.z;

    matrix4 mtx;
    mtx.m11 = 1.0f - (fyy + fzz);
    mtx.m21 = fxy - fwz;
    mtx.m31 = fxz + fwy;
    mtx.m12 = fxy + fwz;
    mtx.m22 = 1.0f - (fxx + fzz);
    mtx.m32 = fyz - fwx;
    mtx.m13 = fxz - fwy;
    mtx.m23 = fyz + fwx;
    mtx.m33 = 1.0f - (fxx + fyy);

    mtx.m41 = mtx.m42 = mtx.m43 = mtx.m14 = mtx.m24 = mtx.m34 = 0;
    mtx.m44 = 1;

    return mtx;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::ortho(float left, float right, float bottom, float top, float z_near, float z_far, bool is_homogenous_depth, bool reversed_z) noexcept
{
    matrix4 m {};

    m.m11 = 2 / (right - left);
    m.m22 = 2 / (top - bottom);
    m.m41 = (right + left) / (left - right);
    m.m42 = (top + bottom) / (bottom - top);

    if(reversed_z)
    {
        m.m33 = (is_homogenous_depth ? 2 : 1) / (z_far - z_near);
        m.m43 = is_homogenous_depth ? (z_near + z_far) / (z_far - z_near) : z_far / (z_far - z_near);
    }
    else
    {
        m.m33 = (is_homogenous_depth ? -2 : -1) / (z_far - z_near);
        m.m43 = is_homogenous_depth ? (z_near + z_far) / (z_near - z_far) : z_near / (z_near - z_far);
    }

    return m;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::from_perspective(float fov, float ratio, float near_plane, float far_plane,
    bool is_homogenous_depth, bool reversed_z) noexcept
{
    matrix4 m {};

    float const f = 1 / tanf(fov * 0.5f);
    float const z_diff = (near_plane - far_plane);

    m.m11 = f / ratio;
    m.m22 = f;
    m.m44 = 0;
    m.m34 = -1.0f;

    if(reversed_z)
    {
        m.m33 = (is_homogenous_depth ? near_plane : -far_plane) / z_diff - (is_homogenous_depth ? 0 : 1);
        m.m43 = is_homogenous_depth ? far_plane * near_plane / z_diff : -near_plane * far_plane / z_diff;
    }
    else
    {
        m.m33 = (is_homogenous_depth ? far_plane + near_plane : far_plane) / z_diff;
        m.m43 = is_homogenous_depth ? 2 * far_plane * near_plane / z_diff : near_plane * far_plane / z_diff;
    }

    return m;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::from_euler(float yaw, float pitch, float roll) noexcept
{
    matrix4 m {};

    float sroll = sinf(roll);
    float croll = cosf(roll);
    float spitch = sinf(pitch);
    float cpitch = cosf(pitch);
    float syaw = sinf(yaw);
    float cyaw = cosf(yaw);

    m.m11 = sroll * spitch * syaw + croll * cyaw;
    m.m12 = sroll * cpitch;
    m.m13 = sroll * spitch * cyaw - croll * syaw;
    m.m14 = 0.0f;
    m.m21 = croll * spitch * syaw - sroll * cyaw;
    m.m22 = croll * cpitch;
    m.m23 = croll * spitch * cyaw + sroll * syaw;
    m.m24 = 0.0f;
    m.m31 = cpitch * syaw;
    m.m32 = -spitch;
    m.m33 = cpitch * cyaw;
    m.m34 = 0.0f;
    m.m41 = 0.0f;
    m.m42 = 0.0f;
    m.m43 = 0.0f;
    m.m44 = 1.0f;

    return m;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::look_at(const vec3f& eye, const vec3f& at, const vec3f& up) noexcept
{
    matrix4 result {};

    vec3f f = eye - at;
    f.normalize();

    vec3f r = cross_product(up, f);
    r.normalize();

    vec3f u = cross_product(f, r);

    result.set_xvec(r);
    result.set_yvec(u);
    result.set_zvec(f);
    result.transpose();
    result.set_translation(vec3f(-dot_product(r, eye), -dot_product(u, eye), -dot_product(f, eye)));

    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void matrix4::decompose(vec3f& position, quaternion& rotation, float& scale) const noexcept
{
    get_translation(position);
    scale = getXVector().length();
    matrix4 tmp = *this;
    tmp.multiply_3x3(1 / scale);
    rotation = tmp.get_rotation();
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::operator *(float rhs) const noexcept
{
    matrix4 out;
    out.m11 = m11 * rhs;
    out.m12 = m12 * rhs;
    out.m13 = m13 * rhs;
    out.m14 = m14 * rhs;

    out.m21 = m21 * rhs;
    out.m22 = m22 * rhs;
    out.m23 = m23 * rhs;
    out.m24 = m24 * rhs;

    out.m31 = m31 * rhs;
    out.m32 = m32 * rhs;
    out.m33 = m33 * rhs;
    out.m34 = m34 * rhs;

    out.m41 = m41 * rhs;
    out.m42 = m42 * rhs;
    out.m43 = m43 * rhs;
    out.m44 = m44 * rhs;

    return out;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::operator +(const matrix4& rhs) const noexcept
{ 
    matrix4 out;

    out.m11 = m11 + rhs.m11;
    out.m12 = m12 + rhs.m12;
    out.m13 = m13 + rhs.m13;
    out.m14 = m14 + rhs.m14;

    out.m21 = m21 + rhs.m21;
    out.m22 = m22 + rhs.m22;
    out.m23 = m23 + rhs.m23;
    out.m24 = m24 + rhs.m24;

    out.m31 = m31 + rhs.m31;
    out.m32 = m32 + rhs.m32;
    out.m33 = m33 + rhs.m33;
    out.m34 = m34 + rhs.m34;

    out.m41 = m41 + rhs.m41;
    out.m42 = m42 + rhs.m42;
    out.m43 = m43 + rhs.m43;
    out.m44 = m44 + rhs.m44;

    return out;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
matrix4 matrix4::operator *(const matrix4& rhs) const noexcept
{
    matrix4 out {};

    __m128 a, b, c, d;
    a = _mm_load_ps(&m11);
    b = _mm_load_ps(&m21);
    c = _mm_load_ps(&m31);
    d = _mm_load_ps(&m41);

    __m128 t1, t2;
    t1 = _mm_set_ps1(rhs.m11);
    t2 = _mm_mul_ps(a, t1);
    t1 = _mm_set_ps1(rhs.m12);
    t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
    t1 = _mm_set_ps1(rhs.m13);
    t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
    t1 = _mm_set_ps1(rhs.m14);
    t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);
    _mm_store_ps(&out.m11, t2);

    t1 = _mm_set_ps1(rhs.m21);
    t2 = _mm_mul_ps(a, t1);
    t1 = _mm_set_ps1(rhs.m22);
    t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
    t1 = _mm_set_ps1(rhs.m23);
    t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
    t1 = _mm_set_ps1(rhs.m24);
    t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);
    _mm_store_ps(&out.m21, t2);

    t1 = _mm_set_ps1(rhs.m31);
    t2 = _mm_mul_ps(a, t1);
    t1 = _mm_set_ps1(rhs.m32);
    t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
    t1 = _mm_set_ps1(rhs.m33);
    t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
    t1 = _mm_set_ps1(rhs.m34);
    t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);
    _mm_store_ps(&out.m31, t2);

    t1 = _mm_set_ps1(rhs.m41);
    t2 = _mm_mul_ps(a, t1);
    t1 = _mm_set_ps1(rhs.m42);
    t2 = _mm_add_ps(_mm_mul_ps(b, t1), t2);
    t1 = _mm_set_ps1(rhs.m43);
    t2 = _mm_add_ps(_mm_mul_ps(c, t1), t2);
    t1 = _mm_set_ps1(rhs.m44);
    t2 = _mm_add_ps(_mm_mul_ps(d, t1), t2);
    _mm_store_ps(&out.m41, t2);

    return out;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void matrix4::normalize_scale() noexcept
{
    vec3f scale
    {
        1 / vec3f(m11, m21, m31).length(),
        1 / vec3f(m12, m22, m32).length(),
        1 / vec3f(m13, m23, m33).length()
    };

    m11 *= scale.x;
    m21 *= scale.x;
    m31 *= scale.x;

    m12 *= scale.y;
    m22 *= scale.y;
    m32 *= scale.y;

    m13 *= scale.z;
    m23 *= scale.z;
    m33 *= scale.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
quaternion matrix4::get_rotation() const noexcept
{
    quaternion rot;
    float const tr = m11 + m22 + m33;

    if(tr > 0)
    {
        float t = tr + 1.0f;
        float s = 1 / sqrt(t) * 0.5f;

        rot.w = s * t;
        rot.z = (m12 - m21) * s;
        rot.y = (m31 - m13) * s;
        rot.x = (m23 - m32) * s;
    }
    else if((m11 > m22) && (m11 > m33))
    {
        float t = 1.0f + m11 - m22 - m33;
        float s = 1 / sqrt(t) * 0.5f;
        rot.x = s * t;
        rot.y = (m12 + m21) * s;
        rot.z = (m13 + m31) * s;
        rot.w = (m23 - m32) * s;
    }
    else if(m22 > m33)
    {
        float t = 1.0f + m22 - m11 - m33;
        float s = 1 / sqrt(t) * 0.5f;
        rot.w = (m31 - m13) * s;
        rot.x = (m12 + m21) * s;
        rot.y = s * t;
        rot.z = (m23 + m32) * s;
    }
    else
    {
        float t = 1.0f + m33 - m11 - m22;
        float s = 1 / sqrt(t) * 0.5f;
        rot.w = (m12 - m21) * s;
        rot.x = (m31 + m13) * s;
        rot.y = (m32 + m23) * s;
        rot.z = s * t;
    }
    return rot;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void matrix4::transpose() noexcept
{
    float tmp = m21;

    m21 = m12;
    m12 = tmp;

    tmp = m32;
    m32 = m23;
    m23 = tmp;

    tmp = m31;
    m31 = m13;
    m13 = tmp;

    tmp = m41;
    m41 = m14;
    m14 = tmp;

    tmp = m42;
    m42 = m24;
    m24 = tmp;

    tmp = m43;
    m43 = m34;
    m34 = tmp;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void matrix4::multiply_3x3(float scale) noexcept
{
    m11 *= scale;
    m12 *= scale;
    m13 *= scale;
    m21 *= scale;
    m22 *= scale;
    m23 *= scale;
    m31 *= scale;
    m32 *= scale;
    m33 *= scale;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
vec3f matrix4::transform_point(const vec3f& rhs) const noexcept
{
    return vec3f
    {
        m11 * rhs.x + m21 * rhs.y + m31 * rhs.z + m41,
        m12 * rhs.x + m22 * rhs.y + m32 * rhs.z + m42,
        m13 * rhs.x + m23 * rhs.y + m33 * rhs.z + m43
    };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
vec3f matrix4::transform_vector(const vec3f& rhs) const noexcept
{
    return vec3f
    {
        m11 * rhs.x + m21 * rhs.y + m31 * rhs.z,
        m12 * rhs.x + m22 * rhs.y + m32 * rhs.z,
        m13 * rhs.x + m23 * rhs.y + m33 * rhs.z
    };
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void matrix4::set_identity() noexcept
{
    m11 = 1; m12 = 0; m13 = 0; m14 = 0;
    m21 = 0; m22 = 1; m23 = 0; m24 = 0;
    m31 = 0; m32 = 0; m33 = 1; m34 = 0;
    m41 = 0; m42 = 0; m43 = 0; m44 = 1;
}

} // namespace xr::math
//-----------------------------------------------------------------------------------------------------------
