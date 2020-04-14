// This file is a part of xray-ng engine
//

#include "corlib/math/vector.h"
#include "corlib/math/mathlib.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::math
{

const vec2f vec2f::MAX { FLT_MAX };
const vec2f vec2f::MIN { -FLT_MAX };
const vec2f vec2f::ZERO { 0 };

const vec3f vec3f::MAX { FLT_MAX };
const vec3f vec3f::MIN { -FLT_MAX };
const vec3f vec3f::ZERO { 0 };

const vec4f vec4f::MAX { FLT_MAX };
const vec4f vec4f::MIN { -FLT_MAX };
const vec4f vec4f::ZERO { 0 };

//-----------------------------------------------------------------------------------------------------------
/**
 */
float vec2f::squared_length() const noexcept
{
    float x = this->x;
    float y = this->y;
    return x * x + y * y;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vec2f::normalize() noexcept
{
    float x = this->x;
    float y = this->y;
    float inv_len = rsqrt(x * x + y * y);
    x *= inv_len;
    y *= inv_len;
    this->x = x;
    this->y = y;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
vec2f vec2f::normalized() const noexcept
{
    float x = this->x;
    float y = this->y;
    float inv_len = rsqrt(x * x + y * y);
    x *= inv_len;
    y *= inv_len;
    return vec2f { x, y };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
float vec2f::length() const noexcept
{
    float x = this->x;
    float y = this->y;
    return sqrt(x * x + y * y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vec3f::normalize() noexcept
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float inv_len = rsqrt(x * x + y * y + z * z);
    x *= inv_len;
    y *= inv_len;
    z *= inv_len;
    this->x = x;
    this->y = y;
    this->z = z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
vec3f vec3f::normalized() const noexcept
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    const float inv_len = rsqrt(x * x + y * y + z * z);
    x *= inv_len;
    y *= inv_len;
    z *= inv_len;
    return vec3f { x, y, z };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
double vec3d::length() const noexcept
{
    const double x = this->x;
    const double y = this->y;
    const double z = this->z;
    return sqrt64(x * x + y * y + z * z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
float vec3f::length() const noexcept
{
    const float x = this->x;
    const float y = this->y;
    const float z = this->z;
    return sqrt(x * x + y * y + z * z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void vec4f::normalize() noexcept
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;
    float inv_len = rsqrt(x * x + y * y + z * z + w * w);
    x *= inv_len;
    y *= inv_len;
    z *= inv_len;
    w *= inv_len;
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
vec4f vec4f::normalized() const noexcept
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;
    float inv_len = rsqrt(x * x + y * y + z * z + w * w);
    x *= inv_len;
    y *= inv_len;
    z *= inv_len;
    w *= inv_len;
    return vec4f { x, y, z, w };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
float vec4f::length() const noexcept
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;
    return sqrt(x * x + y * y + z * z + w * w);
}

} // namespace xr::math
//-----------------------------------------------------------------------------------------------------------
