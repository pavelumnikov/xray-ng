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
    type x {}, y {};
    type width {}, height {};
 
    XR_CONSTEXPR_CPP14_OR_INLINE rect() XR_NOEXCEPT;
}; // struct rect<Dimension>

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE rect<Dimension>::rect() XR_NOEXCEPT
    : x(0)
    , y(0)
    , width(0)
    , height(0)
{}

//-----------------------------------------------------------------------------------------------------------
typedef rect<int32_t> int_rect;
typedef rect<uint32_t> uint_rect;
typedef rect<float> float_rect;

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
