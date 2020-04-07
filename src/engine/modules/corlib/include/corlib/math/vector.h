/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

#pragma once

#include "corlib/types.h"

namespace xr::math
{

struct vec2f;

struct vec2i
{
    vec2i() = default;

    explicit vec2i(int32_t i) : x(i), y(i)
    {}

    explicit inline vec2i(const vec2f& rhs);
    vec2i(int32_t x, int32_t y) : x(x), y(y)
    {}

    vec2i operator +(const vec2i& v) const
    {
        return vec2i(x + v.x, y + v.y);
    }

    vec2i operator -(const vec2i& v) const
    {
        return vec2i(x - v.x, y - v.y);
    }

    vec2i operator *(int32_t i) const
    {
        return vec2i(x * i, y * i);
    }

    vec2i operator / (int32_t rhs)
    {
        return vec2i(x / rhs, y / rhs);
    }

    inline vec2f operator / (const vec2f& rhs);

    bool operator !=(const vec2i& rhs) const
    {
        return rhs.x != x || rhs.y != y;
    }

    int32_t x {}, y {};
};


struct vec3d;

struct vec3i
{
    vec3i() = default;

    vec3i(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z)
    {}

    explicit inline vec3i(const vec3d& rhs);
    bool operator==(const vec3i& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    inline vec3d operator *(double i) const;

    int32_t x {}, y {}, z {};
};


struct vec4i
{
    vec4i() = default;

    vec4i(const vec2i& a, const vec2i& b) : x(a.x), y(a.y), z(b.x), w(b.y)
    {}

    int32_t x {}, y {}, z {}, w {};
};


struct vec2f
{
    vec2f() = default;

    explicit vec2f(const vec2i& rhs)
        : x { float(rhs.x) }
        , y { float(rhs.y) }
    {}

    explicit vec2f(float a)
        : x { a }
        , y { a }
    {}

    vec2f(float a, float b)
        : x(a)
        , y(b)
    {}

    void set(float a, float b)
    {
        x = a;
        y = b;
    }

    template<typename L>
    float& operator[](L i)
    {
        ASSERT(i >= 0 && i < 2);
        return (&x)[i];
    }

    template<typename L>
    float operator[](L i) const
    {
        ASSERT(i >= 0 && i < 2);
        return (&x)[i];
    }

    bool operator==(const vec2f& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    bool operator!=(const vec2f& rhs) const
    {
        return x != rhs.x || y != rhs.y;
    }

    void operator/=(float rhs)
    {
        *this *= 1.0f / rhs;
    }

    void operator*=(float f)
    {
        x *= f;
        y *= f;
    }

    vec2f operator *(const vec2f& v) const
    {
        return vec2f(x * v.x, y * v.y);
    }
    vec2f operator /(const vec2f& v) const
    {
        return vec2f(x / v.x, y / v.y);
    }
    vec2f operator *(float f) const
    {
        return vec2f(x * f, y * f);
    }
    vec2f operator /(float f) const
    {
        return vec2f(x / f, y / f);
    }
    vec2f operator +(const vec2f& v) const
    {
        return vec2f(x + v.x, y + v.y);
    }
    void operator +=(const vec2f& v)
    {
        x += v.x; y += v.y;
    }
    vec2f operator -(const vec2f& v) const
    {
        return vec2f(x - v.x, y - v.y);
    }
    vec2f operator -(float f) const
    {
        return vec2f(x - f, y - f);
    }

    void normalize();
    vec2f normalized() const;
    float length() const;
    float squared_length() const;

    float x {}, y {};

    static const vec2f MAX;
    static const vec2f MIN;
    static const vec2f ZERO;
};


struct vec3f
{
    vec3f() = default;

    vec3f(const vec2f& v, float c)
        : x(v.x)
        , y(v.y)
        , z(c)
    {}

    explicit vec3f(float a)
        : x(a)
        , y(a)
        , z(a)
    {}

    vec3f(float a, float b, float c)
        : x(a)
        , y(b)
        , z(c)
    {}

    template<typename L>
    float& operator[](L i)
    {
        ASSERT(i >= 0 && i < 3);
        return (&x)[i];
    }

    template<typename L>
    float operator[](L i) const
    {
        ASSERT(i >= 0 && i < 3);
        return (&x)[i];
    }

    bool operator==(const vec3f& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    bool operator!=(const vec3f& rhs) const
    {
        return x != rhs.x || y != rhs.y || z != rhs.z;
    }

    vec3f operator+(const vec3f& rhs) const
    {
        return vec3f(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    vec3f operator-() const
    {
        return vec3f(-x, -y, -z);
    }

    vec3f operator-(const vec3f& rhs) const
    {
        return vec3f(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    void operator+=(const vec3f& rhs)
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

    void operator-=(const vec3f& rhs)
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

    vec3f operator*(float s) const
    {
        return vec3f(x * s, y * s, z * s);
    }
    vec3f operator*(const vec3f& rhs) const
    {
        return vec3f(x * rhs.x, y * rhs.y, z * rhs.z);
    }
    vec3f operator*(const vec3i& rhs) const
    {
        return vec3f(x * rhs.x, y * rhs.y, z * rhs.z);
    }
    vec3f operator/(const vec3i& rhs) const
    {
        return vec3f(x / rhs.x, y / rhs.y, z / rhs.z);
    }

    vec3f operator/(float s) const
    {
        float tmp = 1 / s;
        return vec3f(x * tmp, y * tmp, z * tmp);
    }

    void operator/=(float rhs)
    {
        *this *= 1.0f / rhs;
    }

    void operator*=(float rhs)
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

    vec3f normalized() const;

    void normalize();

    void set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float length() const;

    float squared_length() const
    {
        return x * x + y * y + z * z;
    }

    vec2f xz() const
    {
        return { x, z };
    }

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
};


struct vec3d
{
    vec3d() = default;

    vec3d(float a) : x(a), y(a), z(a)
    {}

    vec3d(double x, double y, double z) : x(x), y(y), z(z)
    {}

    vec3d(vec3d const& rhs) = default;

    explicit vec3d(const vec3f& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
    {}

    vec3d operator-() const
    {
        return { -x, -y, -z };
    }

    vec3d operator*(float rhs) const
    {
        return { x * rhs, y * rhs, z * rhs };
    }

    vec3d operator/(float rhs) const
    {
        return { x / rhs, y / rhs, z / rhs };
    }

    vec3d operator-(const vec3d& rhs) const
    {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    vec3d operator+(const vec3d& rhs) const
    {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    vec3d operator-(const vec3f& rhs) const
    {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    vec3d operator+(const vec3f& rhs) const
    {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    double length() const;

    double squared_length() const
    {
        return x * x + y * y + z * z;
    }

    void operator*=(const double& rhs)
    {
        x *= rhs; y *= rhs; z *= rhs;
    }

    void operator+=(const vec3f& rhs)
    {
        x += rhs.x; y += rhs.y; z += rhs.z;
    }

    void operator-=(const vec3f& rhs)
    {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
    }

    vec3f toFloat() const
    {
        return { (float)x, (float)y, (float)z };
    }

    void fromFloat(const vec3f& v)
    {
        x = v.x; y = v.y; z = v.z;
    }

    double x, y, z;
};


vec2i::vec2i(const vec2f& rhs)
    : x(int32_t(rhs.x))
    , y(int32_t(rhs.y))
{}


vec2f vec2i::operator/(const vec2f& rhs)
{
    return { x / rhs.x, y / rhs.y };
}


vec3i::vec3i(const vec3d& rhs)
    : x(int32_t(rhs.x))
    , y(int32_t(rhs.y))
    , z(int32_t(rhs.z))
{}


vec3d vec3i::operator *(double i) const
{
    return { i * x, i * y, i * z };
}


inline vec3f operator *(float f, const vec3f& v)
{
    return vec3f(f * v.x, f * v.y, f * v.z);
}


struct vec4f
{
    vec4f() = default;

    explicit vec4f(float a)
        : x(a)
        , y(a)
        , z(a)
        , w(a)
    {}

    vec4f(float a, float b, float c, float d)
        : x(a)
        , y(b)
        , z(c)
        , w(d)
    {}

    vec4f(const vec2f& v1, const vec2f& v2)
        : x(v1.x)
        , y(v1.y)
        , z(v2.x)
        , w(v2.y)
    {}

    vec4f(const vec3f& v, float d)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(d)
    {}

    vec2f xz() const
    {
        return vec2f(x, z);
    }

    vec2f xy() const
    {
        return vec2f(x, y);
    }

    vec3f xyz() const
    {
        return vec3f(x, y, z);
    }

    vec2f yz() const
    {
        return vec2f(y, z);
    }

    vec3f rgb() const
    {
        return vec3f(x, y, z);
    }

    template<typename L>
    float& operator[](L i)
    {
        ASSERT(i >= 0 && i < 4);
        return (&x)[i];
    }

    template<typename L>
    float operator[](L i) const
    {
        ASSERT(i >= 0 && i < 4);
        return (&x)[i];
    }

    bool operator==(const vec4f& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
    }

    bool operator!=(const vec4f& rhs) const
    {
        return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
    }

    vec4f operator+(const vec4f& rhs) const
    {
        return vec4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    vec4f operator-() const
    {
        return vec4f(-x, -y, -z, -w);
    }

    vec4f operator-(const vec4f& rhs) const
    {
        return vec4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    void operator+=(const vec4f& rhs)
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

    void operator-=(const vec4f& rhs)
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

    void operator/=(float rhs)
    {
        *this *= 1.0f / rhs;
    }

    vec4f operator*(float s)
    {
        return vec4f(x * s, y * s, z * s, w * s);
    }

    void operator*=(float rhs)
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

    void normalize();
    vec4f normalized() const;

    void set(const vec3f& v, float w)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    void set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void set(const vec4f& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
    }

    float length() const;

    float squared_length() const
    {
        float x = this->x;
        float y = this->y;
        float z = this->z;
        float w = this->w;
        return x * x + y * y + z * z + w * w;
    }

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
};


inline vec4f operator *(float f, const vec4f& v)
{
    return vec4f(f * v.x, f * v.y, f * v.z, f * v.w);
}


inline vec4f operator*(const vec4f& v, float s)
{
    return vec4f(v.x * s, v.y * s, v.z * s, v.w * s);
}


inline float dot_product(const vec4f& op1, const vec4f& op2)
{
    return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z + op1.w * op2.w;
}


inline void lerp(const vec4f& op1, const vec4f& op2, vec4f* out, float t)
{
    const float invt = 1.0f - t;
    out->x = op1.x * invt + op2.x * t;
    out->y = op1.y * invt + op2.y * t;
    out->z = op1.z * invt + op2.z * t;
    out->w = op1.w * invt + op2.w * t;
}


inline float dot_product(const vec3f& op1, const vec3f& op2)
{
    return op1.x * op2.x + op1.y * op2.y + op1.z * op2.z;
}


inline vec3f cross_product(const vec3f& op1, const vec3f& op2)
{
    return vec3f(op1.y * op2.z - op1.z * op2.y, op1.z * op2.x - op1.x * op2.z, op1.x * op2.y - op1.y * op2.x);
}


inline vec3d cross_product(const vec3d& op1, const vec3d& op2)
{
    return vec3d(op1.y * op2.z - op1.z * op2.y, op1.z * op2.x - op1.x * op2.z, op1.x * op2.y - op1.y * op2.x);
}


inline vec3f lerp(const vec3f& op1, const vec3f& op2, float t)
{
    vec3f res;
    const float invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    res.z = op1.z * invt + op2.z * t;
    return res;
}


inline vec3d lerp(const vec3d& op1, const vec3d& op2, float t)
{
    vec3d res;
    const float invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    res.z = op1.z * invt + op2.z * t;
    return res;
}


inline vec2f lerp(const vec2f& op1, const vec2f& op2, float t)
{
    vec2f res;
    const float invt = 1.0f - t;
    res.x = op1.x * invt + op2.x * t;
    res.y = op1.y * invt + op2.y * t;
    return res;
}

} // namespace xr::math