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

#include "corlib/packed.h"
#include "corlib/math/mathlib.h"

namespace xr::math::details
{
const int32_t BitSelect0 = 0x00000000;
const int32_t BitSelect1 = 0xFFFFFFFF;

const int32_t BitPermute0x = 0x00010203;
const int32_t BitPermute0y = 0x04050607;
const int32_t BitPermute0z = 0x08090A0B;
const int32_t BitPermute0w = 0x0C0D0E0F;
const int32_t BitPermute1x = 0x10111213;
const int32_t BitPermute1y = 0x14151617;
const int32_t BitPermute1z = 0x18191A1B;
const int32_t BitPermute1w = 0x1C1D1E1F;

const int32_t BitCrMaskCr6 = 0x000000F0;
const int32_t BitCrMaskCr6True = 0x00000080;
const int32_t BitCrMaskCr6False = 0x00000020;
const int32_t BitCrMaskCr6Bounds = BitCrMaskCr6False;

typedef uint16_t halfvec;

struct XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) fvec32
{
    union
    {
        float f[4];
        __m128 v;
    };

    inline operator __m128 (void) const
    {
        return this->v;
    }
    inline operator const float* (void) const
    {
        return this->f;
    }
    inline operator __m128i (void) const
    {
        return _mm_castps_si128(this->v);
    }
    inline operator __m128d (void) const
    {
        return _mm_castps_pd(this->v);
    }
};

struct XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) fveci32
{
    union
    {
        int32_t i[4];
        __m128 v;
    };

    inline operator __m128 (void) const
    {
        return this->v;
    }
    inline operator __m128i (void) const
    {
        return _mm_castps_si128(this->v);
    }
    inline operator __m128d (void) const
    {
        return _mm_castps_pd(this->v);
    }
};

struct XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) fvecu8
{
    union
    {

        uint8_t u[16];
        __m128 v;
    };

    inline operator __m128 (void) const
    {
        return this->v;
    }
    inline operator __m128i (void) const
    {
        return _mm_castps_si128(this->v);
    }
    inline operator __m128d (void) const
    {
        return _mm_castps_pd(this->v);
    }
};

struct XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) fvecu32
{
    union
    {
        uint32_t u[4];
        __m128 v;
    };

    inline operator __m128 (void) const
    {
        return this->v;
    }
    inline operator __m128i (void) const
    {
        return _mm_castps_si128(this->v);
    }
    inline operator __m128d (void) const
    {
        return _mm_castps_pd(this->v);
    }
};

struct XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT) fmatrix
{
    __m128 r[4];

    fmatrix(void)
    {}
    fmatrix(float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44);
    explicit fmatrix(_In_count_c_(16) const float* farray);
    fmatrix& operator = (const fmatrix& rhs);
};

#if defined( XRAY_PLATFORM_WINDOWS )
typedef const __m128 ref_fvec;
typedef const fmatrix& ref_fmat;
typedef const __m128& pass_fvec;
#else
typedef const __m128 ref_fvec;
typedef const fmatrix ref_fmat;
typedef const __m128 pass_fvec;
#endif

const fvec32 g_fSinCoefficients0 = { 1.0f, -0.166666667f, 8.333333333e-3f, -1.984126984e-4f };
const fvec32 g_fSinCoefficients1 = { 2.755731922e-6f, -2.505210839e-8f, 1.605904384e-10f, -7.647163732e-13f };
const fvec32 g_fSinCoefficients2 = { 2.811457254e-15f, -8.220635247e-18f, 1.957294106e-20f, -3.868170171e-23f };
const fvec32 g_fCosCoefficients0 = { 1.0f, -0.5f, 4.166666667e-2f, -1.388888889e-3f };
const fvec32 g_fCosCoefficients1 = { 2.480158730e-5f, -2.755731922e-7f, 2.087675699e-9f, -1.147074560e-11f };
const fvec32 g_fCosCoefficients2 = { 4.779477332e-14f, -1.561920697e-16f, 4.110317623e-19f, -8.896791392e-22f };
const fvec32 g_fTanCoefficients0 = { 1.0f, 0.333333333f, 0.133333333f, 5.396825397e-2f };
const fvec32 g_fTanCoefficients1 = { 2.186948854e-2f, 8.863235530e-3f, 3.592128167e-3f, 1.455834485e-3f };
const fvec32 g_fTanCoefficients2 = { 5.900274264e-4f, 2.391290764e-4f, 9.691537707e-5f, 3.927832950e-5f };
const fvec32 g_fASinCoefficients0 = { -0.05806367563904f, -0.41861972469416f, 0.22480114791621f, 2.17337241360606f };
const fvec32 g_fASinCoefficients1 = { 0.61657275907170f, 4.29696498283455f, -1.18942822255452f, -6.53784832094831f };
const fvec32 g_fASinCoefficients2 = { -1.36926553863413f, -4.48179294237210f, 1.41810672941833f, 5.48179257935713f };
const fvec32 g_fATanCoefficients0 = { 1.0f, 0.333333334f, 0.2f, 0.142857143f };
const fvec32 g_fATanCoefficients1 = { 1.111111111e-1f, 9.090909091e-2f, 7.692307692e-2f, 6.666666667e-2f };
const fvec32 g_fATanCoefficients2 = { 5.882352941e-2f, 5.263157895e-2f, 4.761904762e-2f, 4.347826087e-2f };
const fvec32 g_fSinEstCoefficients = { 1.0f, -1.66521856991541e-1f, 8.199913018755e-3f, -1.61475937228e-4f };
const fvec32 g_fCosEstCoefficients = { 1.0f, -4.95348008918096e-1f, 3.878259962881e-2f, -9.24587976263e-4f };
const fvec32 g_fTanEstCoefficients = { 2.484f, -1.954923183e-1f, 2.467401101f, pi_recip };
const fvec32 g_fATanEstCoefficients = { 7.689891418951e-1f, 1.104742493348f, 8.661844266006e-1f, pi_div_2 };
const fvec32 g_fASinEstCoefficients = { -1.36178272886711f, 2.37949493464538f, -8.08228565650486e-1f, 2.78440142746736e-1f };
const fvec32 g_fASinEstConstants = { 1.00000011921f, pi_div_2, 0.0f, 0.0f };
const fvec32 g_fPiConstants0 = { pi, two_pi, pi_recip, two_pi_recip };
const fvec32 g_fIdentityR0 = { 1.0f, 0.0f, 0.0f, 0.0f };
const fvec32 g_fIdentityR1 = { 0.0f, 1.0f, 0.0f, 0.0f };
const fvec32 g_fIdentityR2 = { 0.0f, 0.0f, 1.0f, 0.0f };
const fvec32 g_fIdentityR3 = { 0.0f, 0.0f, 0.0f, 1.0f };
const fvec32 g_fNegIdentityR0 = { -1.0f, 0.0f, 0.0f, 0.0f };
const fvec32 g_fNegIdentityR1 = { 0.0f, -1.0f, 0.0f, 0.0f };
const fvec32 g_fNegIdentityR2 = { 0.0f, 0.0f, -1.0f, 0.0f };
const fvec32 g_fNegIdentityR3 = { 0.0f, 0.0f, 0.0f, -1.0f };

const fveci32 g_fNegativeZero =
{
    approx_as_int<uint32_t>(0x80000000),
    approx_as_int<uint32_t>(0x80000000),
    approx_as_int<uint32_t>(0x80000000),
    approx_as_int<uint32_t>(0x80000000)
};

const fveci32 g_fNegate3 =
{
    approx_as_int<uint32_t>(0x80000000),
    approx_as_int<uint32_t>(0x80000000),
    approx_as_int<uint32_t>(0x80000000),
    0x00000000
};

const fveci32 g_fMask3 =
{
    approx_as_int<uint32_t>(0xFFFFFFFF),
    approx_as_int<uint32_t>(0xFFFFFFFF),
    approx_as_int<uint32_t>(0xFFFFFFFF),
    0x00000000
};

const fveci32 g_fMaskX =
{
    approx_as_int<uint32_t>(0xFFFFFFFF),
    0x00000000,
    0x00000000,
    0x00000000
};

const fveci32 g_fMaskY =
{
    0x00000000,
    approx_as_int<uint32_t>(0xFFFFFFFF),
    0x00000000,
    0x00000000
};

const fveci32 g_fMaskZ =
{
    0x00000000,
    0x00000000,
    approx_as_int<uint32_t>(0xFFFFFFFF),
    0x00000000
};

const fveci32 g_fMaskW =
{
    0x00000000,
    0x00000000,
    0x00000000,
    approx_as_int<uint32_t>(0xFFFFFFFF)
};

const fvec32 g_fOne = { 1.0f, 1.0f, 1.0f, 1.0f };
const fvec32 g_fOne3 = { 1.0f, 1.0f, 1.0f, 0.0f };
const fvec32 g_fTwo = { 2.0f, 2.0f, 2.0f, 2.0f };
const fvec32 g_fZero = { 0.0f, 0.0f, 0.0f, 0.0f };
const fvec32 g_fNegativeOne = { -1.0f, -1.0f, -1.0f, -1.0f };
const fvec32 g_fOneHalf = { 0.5f, 0.5f, 0.5f, 0.5f };
const fvec32 g_fNegativeOneHalf = { -0.5f, -0.5f, -0.5f, -0.5f };
const fvec32 g_fNegativetwo_pi = { -two_pi, -two_pi, -two_pi, -two_pi };
const fvec32 g_fNegativePi = { -pi, -pi, -pi, -pi };
const fvec32 g_fHalfPi = { pi_div_2, pi_div_2, pi_div_2, pi_div_2 };
const fvec32 g_fPi = { pi, pi, pi, pi };
const fvec32 g_fReciprocalPi = { pi_recip, pi_recip, pi_recip, pi_recip };
const fvec32 g_ftwo_pi = { two_pi, two_pi, two_pi, two_pi };
const fvec32 g_fReciprocaltwo_pi = { two_pi_recip, two_pi_recip, two_pi_recip, two_pi_recip };
const fvec32 g_fEpsilon = { 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f, 1.192092896e-7f };
const fveci32 g_fInfinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
const fveci32 g_fQNaN = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };
const fveci32 g_fQNaNTest = { 0x007FFFFF, 0x007FFFFF, 0x007FFFFF, 0x007FFFFF };
const fveci32 g_fAbsMask = { 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF };
const fveci32 g_fFltMin = { 0x00800000, 0x00800000, 0x00800000, 0x00800000 };
const fveci32 g_fFltMax = { 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF, 0x7F7FFFFF };

const fveci32 g_fNegOneMask =
{
    approx_as_int<uint32_t>(0xFFFFFFFF),
    approx_as_int<uint32_t>(0xFFFFFFFF),
    approx_as_int<uint32_t>(0xFFFFFFFF),
    approx_as_int<uint32_t>(0xFFFFFFFF)
};

const fveci32 g_fMaskA8R8G8B8 =
{
    0x00FF0000,
    0x0000FF00,
    0x000000FF,
    approx_as_int<uint32_t>(0xFF000000)
};

const fveci32 g_fFlipA8R8G8B8 =
{
    0x00000000,
    0x00000000,
    0x00000000, 
    approx_as_int<uint32_t>(0x80000000)
};

const fvec32 g_fFixAA8R8G8B8 =
{
    0.0f,
    0.0f,
    0.0f,
    static_cast<float>(0x80000000U)
};

const fvec32 g_fNormalizeA8R8G8B8 =
{
    1.0f / (255.0f * (float)(0x10000)),
    1.0f / (255.0f * (float)(0x100)),
    1.0f / 255.0f,
    1.0f / (255.0f * (float)(0x1000000))
};

const fveci32 g_fMaskA2B10G10R10 =
{
    0x000003FF,
    0x000FFC00,
    0x3FF00000,
    approx_as_int<uint32_t>(0xC0000000)
};

const fveci32 g_fFlipA2B10G10R10 =
{
    0x00000200,
    0x00080000,
    0x20000000,
    approx_as_int<uint32_t>(0x80000000U)
};

const fvec32 g_fFixAA2B10G10R10 =
{
    -512.0f,
    -512.0f * static_cast<float>(0x400),
    -512.0f * static_cast<float>(0x100000),
    static_cast<float>(0x80000000U)
};

const fvec32 g_fNormalizeA2B10G10R10 =
{
    1.0f / 511.0f,
    1.0f / (511.0f * static_cast<float>(0x400)),
    1.0f / (511.0f * static_cast<float>(0x100000)),
    1.0f / (3.0f * static_cast<float>(0x40000000))
};

const fveci32 g_fMaskX16Y16 =
{
    0x0000FFFF,
    approx_as_int<uint32_t>(0xFFFF0000),
    0x00000000,
    0x00000000
};

const fveci32 g_fFlipX16Y16 = { 0x00008000, 0x00000000, 0x00000000, 0x00000000 };
const fvec32 g_fFixX16Y16 = { -32768.0f, 0.0f, 0.0f, 0.0f };
const fvec32 g_fNormalizeX16Y16 = { 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 0.0f, 0.0f };

const fveci32 g_fMaskX16Y16Z16W16 =
{
    0x0000FFFF,
    0x0000FFFF,
    approx_as_int<uint32_t>(0xFFFF0000),
    approx_as_int<uint32_t>(0xFFFF0000)
};

const fveci32 g_fFlipX16Y16Z16W16 = { 0x00008000, 0x00008000, 0x00000000, 0x00000000 };
const fvec32 g_fFixX16Y16Z16W16 = { -32768.0f, -32768.0f, 0.0f, 0.0f };
const fvec32 g_fNormalizeX16Y16Z16W16 = { 1.0f / 32767.0f, 1.0f / 32767.0f, 1.0f / (32767.0f * 65536.0f), 1.0f / (32767.0f * 65536.0f) };
const fvec32 g_fNoFraction = { 8388608.0f, 8388608.0f, 8388608.0f, 8388608.0f };
const fveci32 g_fMaskByte = { 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF };
const fvec32 g_fNegateX = { -1.0f, 1.0f, 1.0f, 1.0f };
const fvec32 g_fNegateY = { 1.0f, -1.0f, 1.0f, 1.0f };
const fvec32 g_fNegateZ = { 1.0f, 1.0f, -1.0f, 1.0f };
const fvec32 g_fNegateW = { 1.0f, 1.0f, 1.0f, -1.0f };
const fveci32 g_fSelect0101 = { BitSelect0, BitSelect1, BitSelect0, BitSelect1 };
const fveci32 g_fSelect1010 = { BitSelect1, BitSelect0, BitSelect1, BitSelect0 };
const fveci32 g_fOneHalfMinusEpsilon = { 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD, 0x3EFFFFFD };
const fveci32 g_fSelect1000 = { BitSelect1, BitSelect0, BitSelect0, BitSelect0 };
const fveci32 g_fSelect1100 = { BitSelect1, BitSelect1, BitSelect0, BitSelect0 };
const fveci32 g_fSelect1110 = { BitSelect1, BitSelect1, BitSelect1, BitSelect0 };
const fveci32 g_fSwizzleXYXY = { BitPermute0x, BitPermute0y, BitPermute0x, BitPermute0y };
const fveci32 g_fSwizzleXYZX = { BitPermute0x, BitPermute0y, BitPermute0z, BitPermute0x };
const fveci32 g_fSwizzleYXZW = { BitPermute0y, BitPermute0x, BitPermute0z, BitPermute0w };
const fveci32 g_fSwizzleYZXW = { BitPermute0y, BitPermute0z, BitPermute0x, BitPermute0w };
const fveci32 g_fSwizzleZXYW = { BitPermute0z, BitPermute0x, BitPermute0y, BitPermute0w };
const fveci32 g_fPermute0X0Y1X1Y = { BitPermute0x, BitPermute0y, BitPermute1x, BitPermute1y };
const fveci32 g_fPermute0Z0W1Z1W = { BitPermute0z, BitPermute0w, BitPermute1z, BitPermute1w };
const fvec32 g_fFixupY16 = { 1.0f, 1.0f / 65536.0f, 0.0f, 0.0f };
const fvec32 g_fFixupY16W16 = { 1.0f, 1.0f, 1.0f / 65536.0f, 1.0f / 65536.0f };
const fveci32 g_fFlipY = { 0, approx_as_int<uint32_t>(0x80000000), 0, 0 };
const fveci32 g_fFlipZ = { 0, 0, approx_as_int<uint32_t>(0x80000000), 0 };
const fveci32 g_fFlipW = { 0, 0, 0, approx_as_int<uint32_t>(0x80000000) };
const fveci32 g_fFlipYZ = { 0, approx_as_int<uint32_t>(0x80000000), approx_as_int<uint32_t>(0x80000000), 0 };
const fveci32 g_fFlipZW = { 0, 0, approx_as_int<uint32_t>(0x80000000), approx_as_int<uint32_t>(0x80000000) };
const fveci32 g_fFlipYW = { 0, approx_as_int<uint32_t>(0x80000000), 0, approx_as_int<uint32_t>(0x80000000) };
const fveci32 g_fMaskHenD3 = { 0x7FF, 0x7ff << 11, 0x3FF << 22, 0 };
const fveci32 g_fMaskDHen3 = { 0x3FF, 0x7ff << 10, 0x7FF << 21, 0 };
const fvec32 g_fAddUHenD3 = { 0, 0, 32768.0f * 65536.0f, 0 };
const fvec32 g_fAddHenD3 = { -1024.0f, -1024.0f * 2048.0f, 0, 0 };
const fvec32 g_fAddDHen3 = { -512.0f, -1024.0f * 1024.0f, 0, 0 };
const fvec32 g_fMulHenD3 = { 1.0f, 1.0f / 2048.0f, 1.0f / (2048.0f * 2048.0f), 0 };
const fvec32 g_fMulDHen3 = { 1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 2048.0f), 0 };
const fveci32 g_fXorHenD3 = { 0x400, 0x400 << 11, 0, 0 };
const fveci32 g_fXorDHen3 = { 0x200, 0x400 << 10, 0, 0 };
const fveci32 g_fMaskIco4 = { 0xFFFFF, approx_as_int<uint32_t>(0xFFFFF000), 0xFFFFF, approx_as_int<uint32_t>(0xF0000000) };
const fveci32 g_fXorXIco4 = { 0x80000, 0, 0x80000, approx_as_int<uint32_t>(0x80000000) };
const fveci32 g_fXorIco4 = { 0x80000, 0, 0x80000, 0 };
const fvec32 g_fAddXIco4 = { -8.0f * 65536.0f, 0, -8.0f * 65536.0f, 32768.0f * 65536.0f };
const fvec32 g_fAddUIco4 = { 0, 32768.0f * 65536.0f, 0, 32768.0f * 65536.0f };
const fvec32 g_fAddIco4 = { -8.0f * 65536.0f, 0, -8.0f * 65536.0f, 0 };
const fvec32 g_fMulIco4 = { 1.0f, 1.0f / 4096.0f, 1.0f, 1.0f / (4096.0f * 65536.0f) };
const fveci32 g_fMaskDec4 = { 0x3FF, 0x3FF << 10, 0x3FF << 20, 0x3 << 30 };
const fveci32 g_fXorDec4 = { 0x200, 0x200 << 10, 0x200 << 20, 0 };
const fvec32 g_fAddUDec4 = { 0, 0, 0, 32768.0f * 65536.0f };
const fvec32 g_fAddDec4 = { -512.0f, -512.0f * 1024.0f, -512.0f * 1024.0f * 1024.0f, 0 };
const fvec32 g_fMulDec4 = { 1.0f, 1.0f / 1024.0f, 1.0f / (1024.0f * 1024.0f), 1.0f / (1024.0f * 1024.0f * 1024.0f) };
const fveci32 g_fMaskByte4 = { 0xFF, 0xFF00, 0xFF0000, approx_as_int<uint32_t>(0xFF000000) };
const fveci32 g_fXorByte4 = { 0x80, 0x8000, 0x800000, 0x00000000 };
const fvec32 g_fAddByte4 = { -128.0f, -128.0f * 256.0f, -128.0f * 65536.0f, 0 };
const fvec32 g_fFixUnsigned = { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f };
const fvec32 g_fMaxInt = { 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f, 65536.0f * 32768.0f - 128.0f };
const fvec32 g_fMaxUInt = { 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f, 65536.0f * 65536.0f - 256.0f };
const fvec32 g_fUnsignedFix = { 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f, 32768.0f * 65536.0f };

inline __m128 XR_VECTORIZED_CALL FMA(__m128 a, __m128 b, __m128 c)
{
    __m128 result = _mm_mul_ps(b, c);
    result = _mm_add_ps(a, result);
    return result;
}

inline __m128 XR_VECTORIZED_CALL ACosf(__m128 a)
{
    __m128 vTemp1 = _mm_set_ps1(static_cast<float>(0x7fffffff));
    vTemp1 = _mm_and_ps(a, vTemp1);

    __m128 vTemp2 = FMA(_mm_set_ps1(-0.0012624911f), vTemp1, _mm_set_ps1(0.0066700901f));
    vTemp2 = FMA(vTemp2, vTemp1, _mm_set_ps1(-0.0170881256f));
    vTemp2 = FMA(vTemp2, vTemp1, _mm_set_ps1(0.0308918810f));

    __m128 vTemp3 = FMA(_mm_set_ps1(-0.0501743046f), vTemp1, _mm_set_ps1(0.0889789874f));
    vTemp3 = FMA(vTemp3, vTemp1, _mm_set_ps1(-0.2145988016f));
    vTemp3 = FMA(vTemp3, vTemp1, _mm_set_ps1(1.5707963050f));

    __m128 result = _mm_mul_ps(vTemp1, vTemp1);
    result = _mm_mul_ps(result, result);
    result = FMA(vTemp2, result, vTemp3);

    // Adjust the result if cosine angle is negative
    vTemp1 = _mm_sub_ps(_mm_set_ps1(1.0f), vTemp1);
    vTemp1 = _mm_sqrt_ps(vTemp1);

    vTemp2 = _mm_mul_ps(vTemp1, result);
    vTemp3 = _mm_mul_ps(vTemp1, result);
    vTemp3 = _mm_sub_ps(_mm_set_ps1(3.1415926535898f), vTemp3);
    vTemp1 = _mm_cmplt_ps(a, _mm_setzero_ps()); // selection mask

    vTemp2 = _mm_and_ps(vTemp1, vTemp2); // b
    vTemp3 = _mm_andnot_ps(vTemp1, vTemp3); // a
    result = _mm_or_ps(vTemp2, vTemp3);

    return result;
}

inline __m128 Sinf(__m128 a)
{
    __m128 vTemp1 = _mm_mul_ps(a, _mm_set_ps1(0.63661977236f));
    __m128 vTemp2 = _mm_castsi128_ps(_mm_cvtps_epi32(vTemp1));
    __m128 vTemp3 = _mm_and_ps(vTemp2, _mm_set_ps1(static_cast<float>(0x3)));
}

}