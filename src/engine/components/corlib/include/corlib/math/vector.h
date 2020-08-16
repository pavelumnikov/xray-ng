// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

struct vec2f;
struct vec3d;

//-----------------------------------------------------------------------------------------------------------
struct vec2i
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec2i() XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec2i(int32_t i) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec2i(vec2f const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2i(int32_t x, int32_t y) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec2i operator +(vec2i const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2i operator -(vec2i const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2i operator *(int32_t i) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2i operator / (int32_t rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator / (vec2f const& rhs) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE bool operator !=(vec2i const& rhs) const XR_NOEXCEPT;

    int32_t x, y;
}; // struct vec2i

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i::vec2i() XR_NOEXCEPT
    : x(0)
    , y(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i::vec2i(int32_t i) XR_NOEXCEPT
    : x(i)
    , y(i)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i::vec2i(int32_t x, int32_t y) XR_NOEXCEPT
    : x(x)
    , y(y)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i vec2i::operator +(vec2i const& v) const XR_NOEXCEPT
{
    return vec2i(x + v.x, y + v.y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i vec2i::operator -(vec2i const& v) const XR_NOEXCEPT
{
    return vec2i(-v.x, y - v.y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i vec2i::operator *(int32_t i) const XR_NOEXCEPT
{
    return vec2i(x * i, y * i);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i vec2i::operator / (int32_t rhs) XR_NOEXCEPT
{
    return vec2i(x / rhs, y / rhs);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec2i::operator !=(vec2i const& rhs) const XR_NOEXCEPT
{
    return rhs.x != x || rhs.y != y;
}


//-----------------------------------------------------------------------------------------------------------
struct vec3i
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec3i() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3i(int32_t x, int32_t y, int32_t z) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec3i(vec3d const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE bool operator==(vec3i const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator *(double i) const XR_NOEXCEPT;

    int32_t x, y, z;
}; // struct vec3i

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3i::vec3i() XR_NOEXCEPT
    : x(0)
    , y(0)
    , z(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3i::vec3i(int32_t x, int32_t y, int32_t z) XR_NOEXCEPT
    : x(x)
    , y(y)
    , z(z)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec3i::operator==(vec3i const& rhs) const XR_NOEXCEPT
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}


//-----------------------------------------------------------------------------------------------------------
struct vec4i
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec4i() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec4i(vec2i const& a, vec2i const& b) XR_NOEXCEPT;

    int32_t x, y, z, w;
}; // struct vec4i

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4i::vec4i() XR_NOEXCEPT
    : x(0)
    , y(0)
    , z(0)
    , w(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4i::vec4i(vec2i const& a, vec2i const& b) XR_NOEXCEPT
    : x(a.x)
    , y(a.y)
    , z(b.x)
    , w(b.y)
{}


//-----------------------------------------------------------------------------------------------------------
struct vec2f
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f() XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec2f(vec2i const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec2f(float a) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f(float a, float b) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void set(float a, float b) XR_NOEXCEPT;

    template<typename L>
    float& operator[](L i);

    template<typename L>
    float operator[](L i) const;

    XR_CONSTEXPR_CPP14_OR_INLINE bool operator==(vec2f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE bool operator!=(vec2f const& rhs) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void operator +=(vec2f const& v) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator/=(float rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator*=(float f) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator *(vec2f const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator /(vec2f const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator *(float f) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator /(float f) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator +(vec2f const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator -(vec2f const& v) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f operator -(float f) const XR_NOEXCEPT;

    void normalize() XR_NOEXCEPT;
    vec2f normalized() const XR_NOEXCEPT;
    float length() const XR_NOEXCEPT;
    float squared_length() const XR_NOEXCEPT;

    float x, y;

    static const vec2f MAX;
    static const vec2f MIN;
    static const vec2f ZERO;
}; // struct vec2f

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f::vec2f() XR_NOEXCEPT
    : x(0.0f)
    , y(0.0f)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f::vec2f(vec2i const& rhs) XR_NOEXCEPT
    : x(float(rhs.x))
    , y(float(rhs.y))
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f::vec2f(float a) XR_NOEXCEPT
    : x(a)
    , y(a)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f::vec2f(float a, float b) XR_NOEXCEPT
    : x(a)
    , y(b)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec2f::set(float a, float b) XR_NOEXCEPT
{
    x = a;
    y = b;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float& vec2f::operator[](L i)
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 2);
    return (&x)[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float vec2f::operator[](L i) const
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 2);
    return (&x)[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec2f::operator==(vec2f const& rhs) const XR_NOEXCEPT
{
    return x == rhs.x && y == rhs.y;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec2f::operator!=(vec2f const& rhs) const XR_NOEXCEPT
{
    return x != rhs.x || y != rhs.y;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec2f::operator +=(vec2f const& v) XR_NOEXCEPT
{
    x += v.x; y += v.y;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec2f::operator/=(float rhs) XR_NOEXCEPT
{
    *this *= 1.0f / rhs;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec2f::operator*=(float f) XR_NOEXCEPT
{
    x *= f;
    y *= f;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator *(vec2f const& v) const XR_NOEXCEPT
{
    return vec2f(x * v.x, y * v.y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator /(vec2f const& v) const XR_NOEXCEPT
{
    return vec2f(x / v.x, y / v.y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator *(float f) const XR_NOEXCEPT
{
    return vec2f(x * f, y * f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator /(float f) const XR_NOEXCEPT
{
    return vec2f(x / f, y / f);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator +(vec2f const& v) const XR_NOEXCEPT
{
    return vec2f(x + v.x, y + v.y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator -(vec2f const& v) const XR_NOEXCEPT
{
    return vec2f(x - v.x, y - v.y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2f::operator -(float f) const XR_NOEXCEPT
{
    return vec2f(x - f, y - f);
}


//-----------------------------------------------------------------------------------------------------------
struct vec3f
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f(vec2f const& v, float c) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec3f(float a) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f(float a, float b, float c) XR_NOEXCEPT;

    template<typename L>
    float& operator[](L i);

    template<typename L>
    float operator[](L i) const;

    XR_CONSTEXPR_CPP14_OR_INLINE bool operator==(vec3f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE bool operator!=(vec3f const& rhs) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator+(vec3f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator-() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator-(vec3f const& rhs) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void operator+=(vec3f const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator-=(vec3f const& rhs) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator*(float s) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator*(vec3f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator*(vec3i const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator/(vec3i const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator/(float s) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void operator/=(float rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator*=(float rhs) XR_NOEXCEPT;

    vec3f normalized() const XR_NOEXCEPT;
    void normalize() XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void set(float x, float y, float z) XR_NOEXCEPT;

    float length() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE float squared_length() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f xz() const XR_NOEXCEPT;

    union
    {
        struct
        {
            float x, y, z;
        };
        struct
        {
            float r, g, b;
        };
    };

    static const vec3f MAX;
    static const vec3f MIN;
    static const vec3f ZERO;
}; // struct vec3f

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f::vec3f() XR_NOEXCEPT
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f::vec3f(vec2f const& v, float c) XR_NOEXCEPT
    : x(v.x)
    , y(v.y)
    , z(c)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f::vec3f(float a) XR_NOEXCEPT
    : x(a)
    , y(a)
    , z(a)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f::vec3f(float a, float b, float c) XR_NOEXCEPT
    : x(a)
    , y(b)
    , z(c)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float& vec3f::operator[](L i)
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 3);
    return (&x)[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float vec3f::operator[](L i) const
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 3);
    return (&x)[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec3f::operator==(vec3f const& rhs) const XR_NOEXCEPT
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec3f::operator!=(vec3f const& rhs) const XR_NOEXCEPT
{
    return x != rhs.x || y != rhs.y || z != rhs.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator+(vec3f const& rhs) const XR_NOEXCEPT
{
    return vec3f { x + rhs.x, y + rhs.y, z + rhs.z };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator-() const XR_NOEXCEPT
{
    return vec3f { -x, -y, -z };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator-(vec3f const& rhs) const XR_NOEXCEPT
{
    return vec3f { x - rhs.x, y - rhs.y, z - rhs.z };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3f::operator+=(vec3f const& rhs) XR_NOEXCEPT
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    this->x = x;
    this->y = y;
    this->z = z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3f::operator-=(vec3f const& rhs) XR_NOEXCEPT
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    this->x = x;
    this->y = y;
    this->z = z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator*(float s) const XR_NOEXCEPT
{
    return vec3f(x * s, y * s, z * s);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator*(vec3f const& rhs) const XR_NOEXCEPT
{
    return vec3f(x * rhs.x, y * rhs.y, z * rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator*(vec3i const& rhs) const XR_NOEXCEPT
{
    return vec3f(x * rhs.x, y * rhs.y, z * rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator/(vec3i const& rhs) const XR_NOEXCEPT
{
    return vec3f(x / rhs.x, y / rhs.y, z / rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3f::operator/(float s) const XR_NOEXCEPT
{
    float const tmp = 1 / s;
    return vec3f(x * tmp, y * tmp, z * tmp);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3f::operator/=(float rhs) XR_NOEXCEPT
{
    *this *= 1.0f / rhs;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3f::operator*=(float rhs) XR_NOEXCEPT
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    x *= rhs;
    y *= rhs;
    z *= rhs;
    this->x = x;
    this->y = y;
    this->z = z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3f::set(float x, float y, float z) XR_NOEXCEPT
{
    this->x = x;
    this->y = y;
    this->z = z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float vec3f::squared_length() const XR_NOEXCEPT
{
    return x * x + y * y + z * z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec3f::xz() const XR_NOEXCEPT
{
    return { x, z };
}


//-----------------------------------------------------------------------------------------------------------
struct vec3d
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d() XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3d(float a) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d(double x, double y, double z) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec3d(vec3f const& rhs) XR_NOEXCEPT;

    vec3d(vec3d const& rhs) XR_NOEXCEPT = default;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator-() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator*(float rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator/(float rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator-(vec3d const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator+(vec3d const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator-(vec3f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3d operator+(vec3f const& rhs) const XR_NOEXCEPT;

    double length() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE double squared_length() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void operator*=(const double& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator+=(vec3f const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator-=(vec3f const& rhs) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec3f to_float() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void from_float(vec3f const& v) XR_NOEXCEPT;

    double x { 0.0 }, y { 0.0 }, z { 0.0 };
}; // struct vec3d

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d::vec3d() XR_NOEXCEPT
    : x(0.0)
    , y(0.0)
    , z(0.0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d::vec3d(float a) XR_NOEXCEPT
    : x(a)
    , y(a)
    , z(a)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d::vec3d(double x, double y, double z) XR_NOEXCEPT
    : x(x)
    , y(y)
    , z(z)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d::vec3d(const vec3f & rhs) XR_NOEXCEPT
    : x(rhs.x)
    , y(rhs.y)
    , z(rhs.z)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator-() const XR_NOEXCEPT
{
    return vec3d(-x, -y, -z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator*(float rhs) const XR_NOEXCEPT
{
    return { x * rhs, y * rhs, z * rhs };
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator/(float rhs) const XR_NOEXCEPT
{
    return vec3d(x / rhs, y / rhs, z / rhs);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator-(const vec3d & rhs) const XR_NOEXCEPT
{
    return vec3d(x - rhs.x, y - rhs.y, z - rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator+(const vec3d & rhs) const XR_NOEXCEPT
{
    return vec3d(x + rhs.x, y + rhs.y, z + rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator-(const vec3f & rhs) const XR_NOEXCEPT
{
    return vec3d(x - rhs.x, y - rhs.y, z - rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3d::operator+(const vec3f & rhs) const XR_NOEXCEPT
{
    return vec3d(x + rhs.x, y + rhs.y, z + rhs.z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE double vec3d::squared_length() const XR_NOEXCEPT
{
    return x * x + y * y + z * z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3d::operator*=(const double& rhs) XR_NOEXCEPT
{
    x *= rhs; y *= rhs; z *= rhs;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3d::operator+=(const vec3f & rhs) XR_NOEXCEPT
{
    x += rhs.x; y += rhs.y; z += rhs.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3d::operator-=(const vec3f & rhs) XR_NOEXCEPT
{
    x -= rhs.x; y -= rhs.y; z -= rhs.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec3d::to_float() const XR_NOEXCEPT
{
    return vec3f(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec3d::from_float(const vec3f & v) XR_NOEXCEPT
{
    x = v.x; y = v.y; z = v.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2i::vec2i(vec2f const& rhs) XR_NOEXCEPT
    : x(int32_t(rhs.x))
    , y(int32_t(rhs.y))
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec2i::operator/(vec2f const& rhs) XR_NOEXCEPT
{
    return vec2f(x / rhs.x, y / rhs.y);
}


//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3i::vec3i(vec3d const& rhs) XR_NOEXCEPT
    : x(int32_t(rhs.x))
    , y(int32_t(rhs.y))
    , z(int32_t(rhs.z))
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d vec3i::operator *(double i) const XR_NOEXCEPT
{
    return vec3d(i * x, i * y, i * z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f operator *(float f, vec3f const& v) XR_NOEXCEPT
{
    return vec3f(f * v.x, f * v.y, f * v.z);
}


//-----------------------------------------------------------------------------------------------------------
struct vec4f
{
    XR_CONSTEXPR_CPP14_OR_INLINE vec4f() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE explicit vec4f(float a) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec4f(float a, float b, float c, float d) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec4f(vec2f const& v1, vec2f const& v2) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec4f(vec3f const& v, float d) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec2f xz() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f xy() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f xyz() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec2f yz() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec3f rgb() const XR_NOEXCEPT;

    template<typename L>
    float& operator[](L i);

    template<typename L>
    float operator[](L i) const;

    XR_CONSTEXPR_CPP14_OR_INLINE bool operator==(vec4f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE bool operator!=(vec4f const& rhs) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec4f operator+(vec4f const& rhs) const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec4f operator-() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE vec4f operator-(vec4f const& rhs) const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void operator+=(vec4f const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator-=(vec4f const& rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator/=(float rhs) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void operator*=(float rhs) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE vec4f operator*(float s) XR_NOEXCEPT;

    void normalize() XR_NOEXCEPT;
    vec4f normalized() const XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE void set(vec3f const& v, float w) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void set(float x, float y, float z, float w) XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE void set(vec4f const& rhs) XR_NOEXCEPT;

    float length() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE float squared_length() const XR_NOEXCEPT;

    union
    {
        struct
        {
            float x, y, z, w;
        };
        struct
        {
            float r, g, b, a;
        };
    };

    static const vec4f MAX;
    static const vec4f MIN;
    static const vec4f ZERO;
}; // class vec4f

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f::vec4f() XR_NOEXCEPT
    : x(0.0f)
    , y(0.0f)
    , z(0.0f)
    , w(0.0f)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f::vec4f(float a) XR_NOEXCEPT
    : x(a)
    , y(a)
    , z(a)
    , w(a)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f::vec4f(float a, float b, float c, float d) XR_NOEXCEPT
    : x(a)
    , y(b)
    , z(c)
    , w(d)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f::vec4f(vec2f const& v1, vec2f const& v2) XR_NOEXCEPT
    : x(v1.x)
    , y(v1.y)
    , z(v2.x)
    , w(v2.y)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f::vec4f(vec3f const& v, float d) XR_NOEXCEPT
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(d)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec4f::xz() const XR_NOEXCEPT
{
    return vec2f(x, z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec4f::xy() const XR_NOEXCEPT
{
    return vec2f(x, y);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec4f::xyz() const XR_NOEXCEPT
{
    return vec3f(x, y, z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f vec4f::yz() const XR_NOEXCEPT
{
    return vec2f(y, z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f vec4f::rgb() const XR_NOEXCEPT
{
    return vec3f(x, y, z);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float& vec4f::operator[](L i)
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 4);
    return (&x)[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float vec4f::operator[](L i) const
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 4);
    return (&x)[i];
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec4f::operator==(vec4f const& rhs) const XR_NOEXCEPT
{
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE bool vec4f::operator!=(vec4f const& rhs) const XR_NOEXCEPT
{
    return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f vec4f::operator+(vec4f const& rhs) const XR_NOEXCEPT
{
    return vec4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f vec4f::operator-() const XR_NOEXCEPT
{
    return vec4f(-x, -y, -z, -w);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f vec4f::operator-(vec4f const& rhs) const XR_NOEXCEPT
{
    return vec4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::operator+=(vec4f const& rhs) XR_NOEXCEPT
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::operator-=(vec4f const& rhs) XR_NOEXCEPT
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::operator/=(float rhs) XR_NOEXCEPT
{
    *this *= 1.0f / rhs;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f vec4f::operator*(float s) XR_NOEXCEPT
{
    return vec4f(x * s, y * s, z * s, w * s);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::operator*=(float rhs) XR_NOEXCEPT
{
    float x = this->x;
    float y = this->y;
    float z = this->z;
    float w = this->w;
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::set(vec3f const& v, float w) XR_NOEXCEPT
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::set(float x, float y, float z, float w) XR_NOEXCEPT
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void vec4f::set(vec4f const& rhs) XR_NOEXCEPT
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float vec4f::squared_length() const XR_NOEXCEPT
{
    float const x = this->x;
    float const y = this->y;
    float const z = this->z;
    float const w = this->w;
    return x * x + y * y + z * z + w * w;
}


//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f operator *(float f, vec4f const& v) XR_NOEXCEPT
{
    return vec4f(f * v.x, f * v.y, f * v.z, f * v.w);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec4f operator*(vec4f const& v, float s) XR_NOEXCEPT
{
    return vec4f(v.x * s, v.y * s, v.z * s, v.w * s);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float dot_product(vec4f const& op1, vec4f const& op2) XR_NOEXCEPT
{
    return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z + op1.w * op2.w;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE void lerp(vec4f const& op1, vec4f const& op2, vec4f* out, float t) XR_NOEXCEPT
{
    float const invt = 1.0f - t;
    out->x = op1.x * invt + op2.x * t;
    out->y = op1.y * invt + op2.y * t;
    out->z = op1.z * invt + op2.z * t;
    out->w = op1.w * invt + op2.w * t;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE float dot_product(vec3f const& op1, vec3f const& op2) XR_NOEXCEPT
{
    return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f cross_product(vec3f const& op1, vec3f const& op2) XR_NOEXCEPT
{
    return vec3f(
        op1.y * op2.z - op1.z * op2.y,
        op1.z * op2.x - op1.x * op2.z,
        op1.x * op2.y - op1.y * op2.x
    );
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d cross_product(vec3d const& op1, vec3d const& op2) XR_NOEXCEPT
{
    return vec3d(
        op1.y * op2.z - op1.z * op2.y,
        op1.z * op2.x - op1.x * op2.z,
        op1.x * op2.y - op1.y * op2.x
    );
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3f lerp(vec3f const& op1, vec3f const& op2, float t) XR_NOEXCEPT
{
    vec3f res;
    const float invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    res.z = op1.z * invt + op2.z * t;
    return res;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec3d lerp(vec3d const& op1, vec3d const& op2, float t) XR_NOEXCEPT
{
    vec3d res;
    float const invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    res.z = op1.z * invt + op2.z * t;
    return res;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
XR_CONSTEXPR_CPP14_OR_INLINE vec2f lerp(vec2f const& op1, vec2f const& op2, float t) XR_NOEXCEPT
{
    vec2f res;
    float const invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    return res;
}

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
