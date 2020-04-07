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