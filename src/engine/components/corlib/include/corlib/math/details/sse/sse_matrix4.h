// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/math/details/sse/sse_math_intrinsics.h"
#include "corlib/math/details/sse/sse_vector.h"

namespace xr::math::details
{

class XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) sse_matrix4
{
public:
    sse_matrix4(void);
    sse_matrix4(__m128 row1, __m128 row2, __m128 row3, __m128 row4);
    sse_matrix4(sse_vector& row1, sse_vector& row2, sse_vector& row3, sse_vector& row4);

    float determinant() const;
    static sse_matrix4 look_at(const sse_vector& eye, const sse_vector& at, const sse_vector& up);

private:
    fmatrix mx;
};

inline float
sse_matrix4::determinant() const
{

}

inline sse_matrix4
sse_matrix4::look_at(const sse_vector& eye, const sse_vector& at, const sse_vector& up)
{
    sse_vector v1 = up.normalize();
    sse_vector v2 = (eye - at).normalize();
    sse_vector v3 = v1.crossproduct(v2);
    v2 = v2.crossproduct(v3);

    return sse_matrix4(v3, v2, v1, const_cast<sse_vector&>(eye));
}
} // namespace xr::math::details