// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//------------------------------------------------------------------------------
namespace xr::math
{

struct vec2f;
struct vec3d;

//------------------------------------------------------------------------------
struct vec2i
{
    constexpr vec2i() noexcept = default;

    constexpr explicit vec2i(int32_t i) noexcept;
    constexpr explicit vec2i(vec2f const& rhs) noexcept;
    constexpr vec2i(int32_t x, int32_t y) noexcept;

    constexpr vec2i operator +(vec2i const& v) const noexcept;
    constexpr vec2i operator -(vec2i const& v) const noexcept;
    constexpr vec2i operator *(int32_t i) const noexcept;
    constexpr vec2i operator / (int32_t rhs) noexcept;
    constexpr vec2f operator / (vec2f const& rhs) noexcept;

    constexpr bool operator !=(vec2i const& rhs) const noexcept;

    int32_t x {}, y {};
}; // struct vec2i

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i::vec2i(int32_t i) noexcept
    : x { i }
    , y { i }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i::vec2i(int32_t x, int32_t y) noexcept
    : x { x }
    , y { y }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i vec2i::operator +(vec2i const& v) const noexcept
{
    return vec2i { x + v.x, y + v.y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i vec2i::operator -(vec2i const& v) const noexcept
{
    return vec2i { x - v.x, y - v.y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i vec2i::operator *(int32_t i) const noexcept
{
    return vec2i { x * i, y * i };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i vec2i::operator / (int32_t rhs) noexcept
{
    return vec2i { x / rhs, y / rhs };
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec2i::operator !=(vec2i const& rhs) const noexcept
{
    return rhs.x != x || rhs.y != y;
}


//------------------------------------------------------------------------------
struct vec3i
{
    constexpr vec3i() noexcept = default;
    constexpr vec3i(int32_t x, int32_t y, int32_t z) noexcept;

    constexpr explicit vec3i(vec3d const& rhs) noexcept;
    constexpr bool operator==(vec3i const& rhs) const noexcept;
    constexpr vec3d operator *(double i) const noexcept;

    int32_t x {}, y {}, z {};
}; // struct vec3i

//------------------------------------------------------------------------------
/**
 */
constexpr vec3i::vec3i(int32_t x, int32_t y, int32_t z) noexcept
    : x { x }
    , y { y }
    , z { z }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec3i::operator==(vec3i const& rhs) const noexcept
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}


//------------------------------------------------------------------------------
struct vec4i
{
    constexpr vec4i() noexcept = default;
    constexpr vec4i(vec2i const& a, vec2i const& b) noexcept;

    int32_t x {}, y {}, z {}, w {};
}; // struct vec4i

//------------------------------------------------------------------------------
/**
 */
constexpr vec4i::vec4i(vec2i const& a, vec2i const& b) noexcept
    : x { a.x }
    , y { a.y }
    , z { b.x }
    , w { b.y }
{}


//------------------------------------------------------------------------------
struct vec2f
{
    constexpr vec2f() noexcept = default;

    constexpr explicit vec2f(vec2i const& rhs) noexcept;
    constexpr explicit vec2f(float a) noexcept;
    constexpr vec2f(float a, float b) noexcept;

    constexpr void set(float a, float b) noexcept;

    template<typename L>
    float& operator[](L i);

    template<typename L>
    float operator[](L i) const;

    constexpr bool operator==(vec2f const& rhs) const noexcept;
    constexpr bool operator!=(vec2f const& rhs) const noexcept;

    constexpr void operator +=(vec2f const& v) noexcept;
    constexpr void operator/=(float rhs) noexcept;
    constexpr void operator*=(float f) noexcept;

    constexpr vec2f operator *(vec2f const& v) const noexcept;
    constexpr vec2f operator /(vec2f const& v) const noexcept;
    constexpr vec2f operator *(float f) const noexcept;
    constexpr vec2f operator /(float f) const noexcept;

    constexpr vec2f operator +(vec2f const& v) const noexcept;
    constexpr vec2f operator -(vec2f const& v) const noexcept;
    constexpr vec2f operator -(float f) const noexcept;

    void normalize() noexcept;
    vec2f normalized() const noexcept;
    float length() const noexcept;
    float squared_length() const noexcept;

    float x {}, y {};

    static const vec2f MAX;
    static const vec2f MIN;
    static const vec2f ZERO;
}; // struct vec2f

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f::vec2f(vec2i const& rhs) noexcept
    : x { float(rhs.x) }
    , y { float(rhs.y) }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f::vec2f(float a) noexcept
    : x { a }
    , y { a }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f::vec2f(float a, float b) noexcept
    : x { a }
    , y { b }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec2f::set(float a, float b) noexcept
{
    x = a;
    y = b;
}

//------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float& vec2f::operator[](L i)
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 2);
    return (&x)[i];
}

//------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float vec2f::operator[](L i) const
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 2);
    return (&x)[i];
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec2f::operator==(vec2f const& rhs) const noexcept
{
    return x == rhs.x && y == rhs.y;
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec2f::operator!=(vec2f const& rhs) const noexcept
{
    return x != rhs.x || y != rhs.y;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec2f::operator +=(vec2f const& v) noexcept
{
    x += v.x; y += v.y;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec2f::operator/=(float rhs) noexcept
{
    *this *= 1.0f / rhs;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec2f::operator*=(float f) noexcept
{
    x *= f;
    y *= f;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator *(vec2f const& v) const noexcept
{
    return vec2f { x * v.x, y * v.y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator /(vec2f const& v) const noexcept
{
    return vec2f { x / v.x, y / v.y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator *(float f) const noexcept
{
    return vec2f { x * f, y * f };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator /(float f) const noexcept
{
    return vec2f { x / f, y / f };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator +(vec2f const& v) const noexcept
{
    return vec2f { x + v.x, y + v.y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator -(vec2f const& v) const noexcept
{
    return vec2f { x - v.x, y - v.y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2f::operator -(float f) const noexcept
{
    return vec2f { x - f, y - f };
}


//------------------------------------------------------------------------------
struct vec3f
{
    constexpr vec3f() noexcept;
    constexpr vec3f(vec2f const& v, float c) noexcept;
    constexpr explicit vec3f(float a) noexcept;
    constexpr vec3f(float a, float b, float c) noexcept;

    template<typename L>
    float& operator[](L i);

    template<typename L>
    float operator[](L i) const;

    constexpr bool operator==(vec3f const& rhs) const noexcept;
    constexpr bool operator!=(vec3f const& rhs) const noexcept;

    constexpr vec3f operator+(vec3f const& rhs) const noexcept;
    constexpr vec3f operator-() const noexcept;
    constexpr vec3f operator-(vec3f const& rhs) const noexcept;

    constexpr void operator+=(vec3f const& rhs) noexcept;
    constexpr void operator-=(vec3f const& rhs) noexcept;

    constexpr vec3f operator*(float s) const noexcept;
    constexpr vec3f operator*(vec3f const& rhs) const noexcept;
    constexpr vec3f operator*(vec3i const& rhs) const noexcept;
    constexpr vec3f operator/(vec3i const& rhs) const noexcept;
    constexpr vec3f operator/(float s) const noexcept;

    constexpr void operator/=(float rhs) noexcept;
    constexpr void operator*=(float rhs) noexcept;

    vec3f normalized() const noexcept;
    void normalize() noexcept;

    constexpr void set(float x, float y, float z) noexcept;

    float length() const noexcept;
    constexpr float squared_length() const noexcept;
    constexpr vec2f xz() const noexcept;

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

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f::vec3f() noexcept
    : x { 0.0f }
    , y { 0.0f }
    , z { 0.0f }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f::vec3f(vec2f const& v, float c) noexcept
    : x { v.x }
    , y { v.y }
    , z { c }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f::vec3f(float a) noexcept
    : x { a }
    , y { a }
    , z { a }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f::vec3f(float a, float b, float c) noexcept
    : x { a }
    , y { b }
    , z { c }
{}

//------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float& vec3f::operator[](L i)
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 3);
    return (&x)[i];
}

//------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float vec3f::operator[](L i) const
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 3);
    return (&x)[i];
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec3f::operator==(vec3f const& rhs) const noexcept
{
    return x == rhs.x && y == rhs.y && z == rhs.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec3f::operator!=(vec3f const& rhs) const noexcept
{
    return x != rhs.x || y != rhs.y || z != rhs.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator+(vec3f const& rhs) const noexcept
{
    return vec3f { x + rhs.x, y + rhs.y, z + rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator-() const noexcept
{
    return vec3f { -x, -y, -z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator-(vec3f const& rhs) const noexcept
{
    return vec3f { x - rhs.x, y - rhs.y, z - rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3f::operator+=(vec3f const& rhs) noexcept
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

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3f::operator-=(vec3f const& rhs) noexcept
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

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator*(float s) const noexcept
{
    return vec3f { x * s, y * s, z * s };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator*(vec3f const& rhs) const noexcept
{
    return vec3f { x * rhs.x, y * rhs.y, z * rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator*(vec3i const& rhs) const noexcept
{
    return vec3f { x * rhs.x, y * rhs.y, z * rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator/(vec3i const& rhs) const noexcept
{
    return vec3f { x / rhs.x, y / rhs.y, z / rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3f::operator/(float s) const noexcept
{
    float const tmp = 1 / s;
    return vec3f { x * tmp, y * tmp, z * tmp };
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3f::operator/=(float rhs) noexcept
{
    *this *= 1.0f / rhs;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3f::operator*=(float rhs) noexcept
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

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3f::set(float x, float y, float z) noexcept
{
    this->x = x;
    this->y = y;
    this->z = z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr float vec3f::squared_length() const noexcept
{
    return x * x + y * y + z * z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec3f::xz() const noexcept
{
    return { x, z };
}


//------------------------------------------------------------------------------
struct vec3d
{
    constexpr vec3d() noexcept = default;

    constexpr vec3d(float a) noexcept;
    constexpr vec3d(double x, double y, double z) noexcept;
    constexpr explicit vec3d(vec3f const& rhs) noexcept;

    vec3d(vec3d const& rhs) noexcept = default;

    constexpr vec3d operator-() const noexcept;

    constexpr vec3d operator*(float rhs) const noexcept;
    constexpr vec3d operator/(float rhs) const noexcept;
    constexpr vec3d operator-(vec3d const& rhs) const noexcept;
    constexpr vec3d operator+(vec3d const& rhs) const noexcept;
    constexpr vec3d operator-(vec3f const& rhs) const noexcept;
    constexpr vec3d operator+(vec3f const& rhs) const noexcept;

    double length() const noexcept;
    constexpr double squared_length() const noexcept;

    constexpr void operator*=(const double& rhs) noexcept;
    constexpr void operator+=(vec3f const& rhs) noexcept;
    constexpr void operator-=(vec3f const& rhs) noexcept;

    constexpr vec3f to_float() const noexcept;
    constexpr void from_float(vec3f const& v) noexcept;

    double x { 0.0 }, y { 0.0 }, z { 0.0 };
}; // struct vec3d

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d::vec3d(float a) noexcept
    : x { a }
    , y { a }
    , z { a }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d::vec3d(double x, double y, double z) noexcept
    : x { x }
    , y { y }
    , z { z }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d::vec3d(const vec3f & rhs) noexcept
    : x { rhs.x }
    , y { rhs.y }
    , z { rhs.z }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator-() const noexcept
{
    return { -x, -y, -z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator*(float rhs) const noexcept
{
    return { x * rhs, y * rhs, z * rhs };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator/(float rhs) const noexcept
{
    return { x / rhs, y / rhs, z / rhs };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator-(const vec3d & rhs) const noexcept
{
    return { x - rhs.x, y - rhs.y, z - rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator+(const vec3d & rhs) const noexcept
{
    return { x + rhs.x, y + rhs.y, z + rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator-(const vec3f & rhs) const noexcept
{
    return { x - rhs.x, y - rhs.y, z - rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3d::operator+(const vec3f & rhs) const noexcept
{
    return { x + rhs.x, y + rhs.y, z + rhs.z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr double vec3d::squared_length() const noexcept
{
    return x * x + y * y + z * z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3d::operator*=(const double& rhs) noexcept
{
    x *= rhs; y *= rhs; z *= rhs;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3d::operator+=(const vec3f & rhs) noexcept
{
    x += rhs.x; y += rhs.y; z += rhs.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3d::operator-=(const vec3f & rhs) noexcept
{
    x -= rhs.x; y -= rhs.y; z -= rhs.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec3d::to_float() const noexcept
{
    return { static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) };
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec3d::from_float(const vec3f & v) noexcept
{
    x = v.x; y = v.y; z = v.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2i::vec2i(vec2f const& rhs) noexcept
    : x { int32_t(rhs.x) }
    , y { int32_t(rhs.y) }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec2i::operator/(vec2f const& rhs) noexcept
{
    return { x / rhs.x, y / rhs.y };
}


//------------------------------------------------------------------------------
/**
 */
constexpr vec3i::vec3i(vec3d const& rhs) noexcept
    : x { int32_t(rhs.x) }
    , y { int32_t(rhs.y) }
    , z { int32_t(rhs.z) }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d vec3i::operator *(double i) const noexcept
{
    return { i * x, i * y, i * z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f operator *(float f, vec3f const& v) noexcept
{
    return vec3f { f * v.x, f * v.y, f * v.z };
}


//------------------------------------------------------------------------------
struct vec4f
{
    constexpr vec4f() noexcept;
    constexpr explicit vec4f(float a) noexcept;
    constexpr vec4f(float a, float b, float c, float d) noexcept;
    constexpr vec4f(vec2f const& v1, vec2f const& v2) noexcept;
    constexpr vec4f(vec3f const& v, float d) noexcept;

    constexpr vec2f xz() const noexcept;
    constexpr vec2f xy() const noexcept;
    constexpr vec3f xyz() const noexcept;
    constexpr vec2f yz() const noexcept;
    constexpr vec3f rgb() const noexcept;

    template<typename L>
    float& operator[](L i);

    template<typename L>
    float operator[](L i) const;

    constexpr bool operator==(vec4f const& rhs) const noexcept;
    constexpr bool operator!=(vec4f const& rhs) const noexcept;

    constexpr vec4f operator+(vec4f const& rhs) const noexcept;
    constexpr vec4f operator-() const noexcept;
    constexpr vec4f operator-(vec4f const& rhs) const noexcept;

    constexpr void operator+=(vec4f const& rhs) noexcept;
    constexpr void operator-=(vec4f const& rhs) noexcept;
    constexpr void operator/=(float rhs) noexcept;
    constexpr void operator*=(float rhs) noexcept;

    constexpr vec4f operator*(float s) noexcept;

    void normalize() noexcept;
    vec4f normalized() const noexcept;

    constexpr void set(vec3f const& v, float w) noexcept;
    constexpr void set(float x, float y, float z, float w) noexcept;
    constexpr void set(vec4f const& rhs) noexcept;

    float length() const noexcept;
    constexpr float squared_length() const noexcept;

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

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f::vec4f(float a) noexcept
    : x { a }
    , y { a }
    , z { a }
    , w { a }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f::vec4f(float a, float b, float c, float d) noexcept
    : x { a }
    , y { b }
    , z { c }
    , w { d }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f::vec4f(vec2f const& v1, vec2f const& v2) noexcept
    : x { v1.x }
    , y { v1.y }
    , z { v2.x }
    , w { v2.y }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f::vec4f(vec3f const& v, float d) noexcept
    : x { v.x }
    , y { v.y }
    , z { v.z }
    , w { d }
{}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec4f::xz() const noexcept
{
    return vec2f { x, z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec4f::xy() const noexcept
{
    return vec2f { x, y };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec4f::xyz() const noexcept
{
    return vec3f { x, y, z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f vec4f::yz() const noexcept
{
    return vec2f { y, z };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f vec4f::rgb() const noexcept
{
    return vec3f { x, y, z };
}

//------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float& vec4f::operator[](L i)
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 4);
    return (&x)[i];
}

//------------------------------------------------------------------------------
/**
 */
template<typename L>
inline float vec4f::operator[](L i) const
{
    XR_DEBUG_ASSERTION(i >= 0 && i < 4);
    return (&x)[i];
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec4f::operator==(vec4f const& rhs) const noexcept
{
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

//------------------------------------------------------------------------------
/**
 */
constexpr bool vec4f::operator!=(vec4f const& rhs) const noexcept
{
    return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f vec4f::operator+(vec4f const& rhs) const noexcept
{
    return vec4f { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f vec4f::operator-() const noexcept
{
    return vec4f { -x, -y, -z, -w };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f vec4f::operator-(vec4f const& rhs) const noexcept
{
    return vec4f { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::operator+=(vec4f const& rhs) noexcept
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

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::operator-=(vec4f const& rhs) noexcept
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

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::operator/=(float rhs) noexcept
{
    *this *= 1.0f / rhs;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f vec4f::operator*(float s) noexcept
{
    return vec4f { x * s, y * s, z * s, w * s };
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::operator*=(float rhs) noexcept
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

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::set(vec3f const& v, float w) noexcept
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::set(float x, float y, float z, float w) noexcept
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void vec4f::set(vec4f const& rhs) noexcept
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    w = rhs.w;
}

//------------------------------------------------------------------------------
/**
 */
constexpr float vec4f::squared_length() const noexcept
{
    float const x = this->x;
    float const y = this->y;
    float const z = this->z;
    float const w = this->w;
    return x * x + y * y + z * z + w * w;
}


//------------------------------------------------------------------------------
/**
 */
constexpr vec4f operator *(float f, vec4f const& v) noexcept
{
    return vec4f { f * v.x, f * v.y, f * v.z, f * v.w };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec4f operator*(vec4f const& v, float s) noexcept
{
    return vec4f { v.x * s, v.y * s, v.z * s, v.w * s };
}

//------------------------------------------------------------------------------
/**
 */
constexpr float dot_product(vec4f const& op1, vec4f const& op2) noexcept
{
    return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z + op1.w * op2.w;
}

//------------------------------------------------------------------------------
/**
 */
constexpr void lerp(vec4f const& op1, vec4f const& op2, vec4f* out, float t) noexcept
{
    float const invt = 1.0f - t;
    out->x = op1.x * invt + op2.x * t;
    out->y = op1.y * invt + op2.y * t;
    out->z = op1.z * invt + op2.z * t;
    out->w = op1.w * invt + op2.w * t;
}

//------------------------------------------------------------------------------
/**
 */
constexpr float dot_product(vec3f const& op1, vec3f const& op2) noexcept
{
    return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f cross_product(vec3f const& op1, vec3f const& op2) noexcept
{
    return vec3f 
    { 
        op1.y * op2.z - op1.z * op2.y,
        op1.z * op2.x - op1.x * op2.z,
        op1.x * op2.y - op1.y * op2.x
    };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d cross_product(vec3d const& op1, vec3d const& op2) noexcept
{
    return vec3d 
    { 
        op1.y * op2.z - op1.z * op2.y,
        op1.z * op2.x - op1.x * op2.z,
        op1.x * op2.y - op1.y * op2.x
    };
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3f lerp(vec3f const& op1, vec3f const& op2, float t) noexcept
{
    vec3f res;
    const float invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    res.z = op1.z * invt + op2.z * t;
    return res;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec3d lerp(vec3d const& op1, vec3d const& op2, float t) noexcept
{
    vec3d res;
    float const invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    res.z = op1.z * invt + op2.z * t;
    return res;
}

//------------------------------------------------------------------------------
/**
 */
constexpr vec2f lerp(vec2f const& op1, vec2f const& op2, float t) noexcept
{
    vec2f res;
    float const invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    return res;
}

} // namespace xr::math
//------------------------------------------------------------------------------