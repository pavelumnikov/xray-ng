// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

//-----------------------------------------------------------------------------------------------------------
template<typename Dimension>
struct cube final
{
    typedef Dimension type;
    XR_CONSTEXPR_CPP14_OR_INLINE cube(type top, type bottom, type left, type right, type front, type back)
        : top(top), bottom(bottom), left(left), right(right), front(front), back(back)
    {}

    type top, bottom, left, right, front, back;
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename Dimension>
XR_CONSTEXPR_CPP14_OR_INLINE cube<Dimension>::cube(type top, type bottom,
    type left, type right, type front, type back)
    : top(top)
    , bottom(bottom)
    , left(left)
    , right(right)
    , front(front)
    , back(back)
{}

//-----------------------------------------------------------------------------------------------------------
typedef cube<int32_t> int_cube;
typedef cube<uint32_t> uint_cube;
typedef cube<float> float_cube;

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------
