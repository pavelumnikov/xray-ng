// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/mathlib.h"
#include "EASTL/algorithm.h"

namespace xr::math
{
// maps unsigned 8 bits/channel to dword color

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_argb(uint32_t a, uint32_t r, uint32_t g, uint32_t b)
{
    return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_rgba(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
    return color_argb(a, r, g, b);
}

//------------------------------------------------------------------------------
/**
 */
inline uint32_t
color_argb_f(float a, float r, float g, float b)
{
    int32_t ir = eastl::clamp(ifloor(r * 255.f), 0, 255);
    int32_t ig = eastl::clamp(ifloor(g * 255.f), 0, 255);
    int32_t ib = eastl::clamp(ifloor(b * 255.f), 0, 255);
    int32_t ia = eastl::clamp(ifloor(a * 255.f), 0, 255);
    return color_argb(ia, ir, ig, ib);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_rgba_f(float r, float g, float b, float a)
{
    return color_argb_f(a, r, g, b);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_xrgb(uint32_t r, uint32_t g, uint32_t b)
{
    return color_argb(0xff, r, g, b);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_get_R(uint32_t rgba)
{
    return (((rgba) >> 16) & 0xff);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_get_G(uint32_t rgba)
{
    return (((rgba) >> 8) & 0xff);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
color_get_B(uint32_t rgba)
{
    return ((rgba) & 0xff);
}

//------------------------------------------------------------------------------
/**
 */
inline uint32_t
color_get_A(uint32_t rgba)
{
    return ((rgba) >> 24);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
subst_alpha(uint32_t rgba, uint32_t a)
{
    return rgba & ~color_rgba(0, 0, 0, 0xff) | color_rgba(0, 0, 0, a);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
bgr_to_rgb(uint32_t bgr)
{
    return color_rgba(color_get_B(bgr), color_get_G(bgr), color_get_R(bgr), 0);
}

//------------------------------------------------------------------------------
/**
 */
constexpr uint32_t
rgb_to_bgr(uint32_t rgb)
{
    return bgr_to_rgb(rgb);
}

//------------------------------------------------------------------------------
template<typename T>
struct base_color
{
    using value_type = T;
    using this_type = base_color<value_type>;
    value_type r, g, b, a;

    this_type& set(uint32_t dw);
    this_type& set(value_type _r, value_type _g, value_type _b, value_type _a);
    this_type& set(this_type const& dw);
    uint32_t get() const;

    uint32_t get_windows() const;
    this_type& set_windows(uint32_t dw);

    this_type& adjust_contrast(value_type f); // >1 - contrast will be increased
    this_type& adjust_contrast(this_type const& in, value_type f); // >1 - contrast will be increased

    this_type& adjust_saturation(value_type s);
    this_type& adjust_saturation(this_type const& in, value_type s);

    this_type& modulate(base_color& in);
    this_type& modulate(this_type const& in1, this_type const& in2);

    this_type& negative(this_type const& in);
    this_type& negative();

    this_type& sub_rgb(value_type s);

    this_type& add_rgb(value_type s);
    this_type& add_rgba(value_type s);

    this_type& mul_rgba(value_type s);
    this_type&	mul_rgb(value_type s);
    this_type& mul_rgba(this_type const& c, value_type s);
    this_type& mul_rgb(this_type const& c, value_type s);

    // SQ magnitude
    value_type magnitude_sqr_rgb() const;

    // magnitude
    value_type magnitude_rgb() const;

    value_type intensity() const;

    // Normalize
    this_type& normalize_rgb();
    this_type& normalize_rgb(this_type const& c);

    this_type& lerp(this_type const& c1, this_type const& c2, value_type t);
    this_type& lerp(this_type const& c1, this_type const& c2, this_type const& c3, value_type t);

    bool similar_rgba(this_type const& v, value_type E = EPS_L) const;
    bool similar_rgb(this_type const& v, value_type E = EPS_L) const;
}; // struct base_color<T>

using Fcolor = base_color<float>;
using Dcolor = base_color<double>;

//------------------------------------------------------------------------------
/**
 */
template <class _T>
bool valid(const base_color<_T>& c)
{
    return valid(c.r) && valid(c.g) && valid(c.b) && valid(c.a);
}


//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::set(uint32_t dw)
{
    const value_type f = value_type(1.0) / value_type(255.0);
    a = f * value_type((dw >> 24) & 0xff);
    r = f * value_type((dw >> 16) & 0xff);
    g = f * value_type((dw >> 8) & 0xff);
    b = f * value_type((dw >> 0) & 0xff);
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::set(value_type _r, value_type _g, value_type _b, value_type _a)
{
    r = _r; g = _g; b = _b; a = _a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type& 
base_color<T>::set(this_type const& dw)
{
    r = dw.r; g = dw.g; b = dw.b; a = dw.a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline uint32_t
base_color<T>::get() const
{
    return color_rgba_f(r, g, b, a);
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline uint32_t
base_color<T>::get_windows() const
{
    uint8_t _a, _r, _g, _b;
    _a = (uint8_t)(a * 255.f);
    _r = (uint8_t)(r * 255.f);
    _g = (uint8_t)(g * 255.f);
    _b = (uint8_t)(b * 255.f);
    return ((uint32_t)(_a << 24) | (_b << 16) | (_g << 8) | (_r));
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::set_windows(uint32_t dw)
{
    const value_type f = 1.0f / 255.0f;
    a = f * (value_type)(uint8_t)(dw >> 24);
    b = f * (value_type)(uint8_t)(dw >> 16);
    g = f * (value_type)(uint8_t)(dw >> 8);
    r = f * (value_type)(uint8_t)(dw >> 0);
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::adjust_contrast(value_type f) // >1 - contrast will be increased
{
    r = 0.5f + f * (r - 0.5f);
    g = 0.5f + f * (g - 0.5f);
    b = 0.5f + f * (b - 0.5f);
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::adjust_contrast(this_type const& in, value_type f) // >1 - contrast will be increased
{
    r = 0.5f + f * (in.r - 0.5f);
    g = 0.5f + f * (in.g - 0.5f);
    b = 0.5f + f * (in.b - 0.5f);
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::adjust_saturation(value_type s)
{
    // Approximate values for each component's contribution to luminance.
    // Based upon the NTSC standard described in ITU-R Recommendation BT.709.
    value_type grey = r * 0.2125f + g * 0.7154f + b * 0.0721f;

    r = grey + s * (r - grey);
    g = grey + s * (g - grey);
    b = grey + s * (b - grey);
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::adjust_saturation(this_type const& in, value_type s)
{
    // Approximate values for each component's contribution to luminance.
    // Based upon the NTSC standard described in ITU-R Recommendation BT.709.
    value_type grey = in.r * 0.2125f + in.g * 0.7154f + in.b * 0.0721f;

    r = grey + s * (in.r - grey);
    g = grey + s * (in.g - grey);
    b = grey + s * (in.b - grey);
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::modulate(base_color& in)
{
    r *= in.r;
    g *= in.g;
    b *= in.b;
    a *= in.a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::modulate(this_type const& in1, this_type const& in2)
{
    r = in1.r * in2.r;
    g = in1.g * in2.g;
    b = in1.b * in2.b;
    a = in1.a * in2.a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::negative(this_type const& in)
{
    r = 1.0f - in.r;
    g = 1.0f - in.g;
    b = 1.0f - in.b;
    a = 1.0f - in.a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type& 
base_color<T>::negative()
{
    r = 1.0f - r;
    g = 1.0f - g;
    b = 1.0f - b;
    a = 1.0f - a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::sub_rgb(value_type s)
{
    r -= s;
    g -= s;
    b -= s;
    // a=1.0f-a;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::add_rgb(value_type s)
{
    r += s;
    g += s;
    b += s;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::add_rgba(value_type s)
{
    r += s;
    g += s;
    b += s;
    a += s;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::mul_rgba(value_type s)
{
    r *= s;
    g *= s;
    b *= s;
    a *= s;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::mul_rgb(value_type s)
{
    r *= s;
    g *= s;
    b *= s;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::mul_rgba(this_type const& c, value_type s)
{
    r = c.r * s;
    g = c.g * s;
    b = c.b * s;
    a = c.a * s;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::mul_rgb(this_type const& c, value_type s)
{
    r = c.r * s;
    g = c.g * s;
    b = c.b * s;
    return *this;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::value_type
base_color<T>::magnitude_sqr_rgb() const
{
    return r * r + g * g + b * b;
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::value_type
base_color<T>::magnitude_rgb() const
{
    return _sqrt(magnitude_sqr_rgb());
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::value_type
base_color<T>::intensity() const
{
    return (r + g + b) / 3.f;
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::normalize_rgb()
{
    VERIFY(magnitude_sqr_rgb() > EPS_S);
    return mul_rgb(1.f / magnitude_rgb());
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::normalize_rgb(this_type const& c)
{
    VERIFY(c.magnitude_sqr_rgb() > EPS_S);
    return mul_rgb(c, 1.f / c.magnitude_rgb());
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::lerp(this_type const& c1, this_type const& c2, value_type t)
{
    value_type invt = 1.f - t;
    r = c1.r * invt + c2.r * t;
    g = c1.g * invt + c2.g * t;
    b = c1.b * invt + c2.b * t;
    a = c1.a * invt + c2.a * t;
    return *this;
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline base_color<T>::this_type&
base_color<T>::lerp(this_type const& c1, this_type const& c2, this_type const& c3, value_type t)
{
    if(t > .5f)
    {
        return lerp(c2, c3, t * 2.f - 1.f);
    }
    else
    {
        return lerp(c1, c2, t * 2.f);
    }
}

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline bool
base_color<T>::similar_rgba(this_type const& v, 
    value_type E = static_cast<value_type>(eps_l)) const
{
    return _abs(r - v.r) < E && _abs(g - v.g) < E && _abs(b - v.b) < E && _abs(a - v.a) < E;
};

//------------------------------------------------------------------------------
/**
 */
template<typename T>
inline bool
base_color<T>::similar_rgb(this_type const& v,
    value_type E = static_cast<value_type>(eps_l)) const
{
    return _abs(r - v.r) < E && _abs(g - v.g) < E && _abs(b - v.b) < E;
};

} // namespace xr::math
//------------------------------------------------------------------------------