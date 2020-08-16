// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

//-----------------------------------------------------------------------------------------------------------
template<typename Dimension>
struct rect final
{
    typedef Dimension type; 

    XR_CONSTEXPR_CPP14_OR_INLINE rect() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE rect(type top, type bottom, type left, type right) XR_NOEXCEPT;

    XR_CONSTEXPR_CPP14_OR_INLINE type width() const;
    XR_CONSTEXPR_CPP14_OR_INLINE type height() const;

    XR_CONSTEXPR_CPP14_OR_INLINE type center_x() const;
    XR_CONSTEXPR_CPP14_OR_INLINE type center_y() const;

    XR_CONSTEXPR_CPP14_OR_INLINE type area() const;

    type top, bottom, left, right;
}; // struct rect<Dimension>

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE rect<Dimension>::rect() XR_NOEXCEPT
    : top(0)
    , bottom(0)
    , left(0)
    , right(0)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE rect<Dimension>::rect(type top, type bottom, type left, type right) XR_NOEXCEPT
    : top(top)
    , bottom(bottom)
    , left(left)
    , right(right)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE typename rect<Dimension>::type 
rect<Dimension>::width() const
{
    return right - left;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE typename rect<Dimension>::type
rect<Dimension>::height() const
{
    return top - bottom;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE typename rect<Dimension>::type
rect<Dimension>::center_x() const
{
    return (right + left) / 2;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE typename rect<Dimension>::type
rect<Dimension>::center_y() const
{
    return (top + bottom) / 2;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE typename rect<Dimension>::type
rect<Dimension>::area() const
{
    return width() * height();
}

//-----------------------------------------------------------------------------------------------------------
typedef rect<int32_t> int_rect;
typedef rect<uint32_t> uint_rect;
typedef rect<float> float_rect;

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
