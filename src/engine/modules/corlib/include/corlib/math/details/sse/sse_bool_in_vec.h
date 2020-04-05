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