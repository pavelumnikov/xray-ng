// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/details/sse/sse_math_intrinsics.h"

namespace xr::math::details
{

class XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) bool_in_vec
{
public:
    bool_in_vec(void);
    bool_in_vec(__m128 v);
    bool_in_vec(const bool_in_vec& rhs);
    explicit bool_in_vec(bool val);

    operator bool(void) const;

private:
    union intfloat
    {
        int i;
        float f;
    };

    union boolvec
    {
        bool b[4];
        __m128 v;
    };

    __m128 myVec;
};

inline bool_in_vec::bool_in_vec(void)
{
    static const intfloat i = { 0 };
    this->myVec = _mm_set_ps1(i.f);
}

inline bool_in_vec::bool_in_vec(__m128 v) : myVec(v)
{}

inline bool_in_vec::bool_in_vec(const bool_in_vec& rhs) : myVec(rhs.myVec)
{}

inline bool_in_vec::bool_in_vec(bool val)
{
    intfloat i = { -(int)val };
    this->myVec = _mm_set_ps1(i.f);
}

inline bool_in_vec::operator bool(void) const
{
    boolvec b;
    b.v = this->myVec;
    return b.b[0];
}

} // namespace xr::math::details