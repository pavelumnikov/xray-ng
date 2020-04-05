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

#include "corlib/math/details/sse/sse_bool_in_vec.h"

namespace xr::math::details
{

enum EFloatsFromVec
{
    GET_FVEC_FROM_X = 0,
    GET_FVEC_FROM_Y = 1,
    GET_FVEC_FROM_Z = 2,
    GET_FVEC_FROM_W = 3
};

__declspec(align(16))
class float_in_vec
{
public:
    float_in_vec();
    float_in_vec(__m128 v);
    explicit float_in_vec(float f);

    float_in_vec& operator = (const float_in_vec& rhs);

    operator float() const;
    operator __m128() const;
    operator __m128i() const;
    operator __m128d() const;

    const float_in_vec operator ++ (int);
    const float_in_vec operator -- (int);
    float_in_vec& operator ++ (void);
    float_in_vec& operator -- (void);
    const float_in_vec operator - (void) const;
    float_in_vec& operator += (const float_in_vec& vec);
    float_in_vec& operator -= (const float_in_vec& vec);

    template< EFloatsFromVec slot >
    static float_in_vec select_part(__m128 v);

private:
    __m128 myVec;
};

inline float_in_vec::float_in_vec() : myVec(_mm_set_ps1(0.0f))
{}

inline float_in_vec::float_in_vec(__m128 v) : myVec(v)
{}

inline float_in_vec::float_in_vec(float f) : myVec(_mm_set_ps1(f))
{}

inline float_in_vec::operator float(void) const
{
    return _mm_cvtss_f32(this->myVec);
}

inline float_in_vec::operator __m128(void) const
{
    return this->myVec;
}

inline float_in_vec::operator __m128i(void) const
{
    return _mm_castps_si128(this->myVec);
}

inline float_in_vec::operator __m128d(void) const
{
    return _mm_castps_pd(this->myVec);
}

inline const float_in_vec float_in_vec::operator ++ (int)
{
    __m128 old = this->myVec;
    __m128 one = _mm_set_ps1(1.0f);
    this->myVec = _mm_add_ps(this->myVec, one);
    return old;
}

inline const float_in_vec float_in_vec::operator -- (int)
{
    __m128 old = this->myVec;
    __m128 one = _mm_set_ps1(1.0f);
    this->myVec = _mm_sub_ps(this->myVec, one);
    return old;
}

inline float_in_vec& float_in_vec::operator ++ (void)
{
    __m128 one = _mm_set_ps1(1.0f);
    this->myVec = _mm_add_ps(this->myVec, one);
    return *this;
}

inline float_in_vec& float_in_vec::operator -- (void)
{
    __m128 one = _mm_set_ps1(1.0f);
    this->myVec = _mm_sub_ps(this->myVec, one);
    return *this;
}

inline const float_in_vec float_in_vec::operator - (void) const
{
    /*__m128 vZero = _mm_set_ps1( 0.0f );
    __m128 vResult = this->myVec;
    vResult = _mm_sub_ps( vZero, vResult );
    return vResult;*/
    // [Codepoet]: this is faster than subtraction method
    __m128 vResult = _mm_xor_ps(this->myVec, g_fNegativeZero);
    return vResult;
}

inline float_in_vec& float_in_vec::operator += (const float_in_vec& vec)
{
    this->myVec = _mm_add_ps(this->myVec, vec.myVec);
    return *this;
}

inline float_in_vec& float_in_vec::operator -= (const float_in_vec& vec)
{
    this->myVec = _mm_sub_ps(this->myVec, vec.myVec);
    return *this;
}

template< EFloatsFromVec slot >
inline float_in_vec float_in_vec::select_part(__m128 v)
{
    const unsigned int i = (((slot) << 6) | ((slot) << 4) | ((slot) << 2) | ((slot)));
    __m128 shuf = _mm_shuffle_ps(v, v, i);
    return shuf;
}

inline const float_in_vec operator * (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_mul_ps(vec0, vec1);
}

inline const float_in_vec operator / (const float_in_vec& num, const float_in_vec& den)
{
    return _mm_div_ps(num, den);
}

inline const float_in_vec operator + (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_add_ps(vec0, vec1);
}

inline const float_in_vec operator - (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_sub_ps(vec0, vec1);
}

inline const bool_in_vec operator < (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_cmpgt_ps(vec1, vec0);
}

inline const bool_in_vec operator <= (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_cmpge_ps(vec1, vec0);
}

inline const bool_in_vec operator > (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_cmpgt_ps(vec0, vec1);
}

inline const bool_in_vec operator >= (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_cmpge_ps(vec0, vec1);
}

inline const bool_in_vec operator == (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_cmpeq_ps(vec0, vec1);
}

inline const bool_in_vec operator != (const float_in_vec& vec0, const float_in_vec& vec1)
{
    return _mm_cmpneq_ps(vec0, vec1);
}

} // namespace Math