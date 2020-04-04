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

#include "corlib/memory/memory_functions.h"
#include "corlib/etl/algorithms/aligning.h"
#include <string.h>
#include <intrin.h>

//------------------------------------------------------------------------------
namespace xr::memory
{

//------------------------------------------------------------------------------
/**
*/
void copy(void* destination, size_t const destination_size, 
    void const* const source, size_t const size)
{
    XR_UNREFERENCED_PARAMETER(destination_size);
    XR_DEBUG_ASSERTION(destination_size >= size);
    memcpy(destination, source, size);
}

//------------------------------------------------------------------------------
/**
*/
void copy_align_16(void* destination, size_t const destination_size, 
    void const* source, size_t const source_size)
{
    XR_DEBUG_ASSERTION_MSG(source, "Source data must be provided to copy from");
    XR_DEBUG_ASSERTION_MSG(destination, "Destination point must be valid!");

    XR_DEBUG_ASSERTION_MSG(source_size, 
        "Size of copyable data must be higher than zero!");

    XR_DEBUG_ASSERTION_MSG(destination_size, 
        "Size of copyable data must be higher than zero!");

    XR_DEBUG_ASSERTION(etl::algorithms::is_aligned(source, 16));
    XR_DEBUG_ASSERTION(etl::algorithms::is_aligned(destination, 16));

    // To prevent stack growing(useful in cases like multitasking)
    auto const src = const_cast<char*>(static_cast<const char*>(source));
    auto const dst = static_cast<char*>(destination);

    size_t i = 0;
    for(; i + 128 <= destination_size; i += 128)
    {
        // There are 8 XMM registers, so they will be automatically placed in 
        // register memory that's why don't need to place register modifier here

        auto const d0 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 0 * 16]));

        auto const d1 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 1 * 16]));

        auto const d2 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 2 * 16]));

        auto const d3 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 3 * 16]));

        auto const d4 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 4 * 16]));

        auto const d5 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 5 * 16]));

        auto const d6 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 6 * 16]));

        auto const d7 = 
            _mm_load_si128(reinterpret_cast<__m128i*>(&src[i + 7 * 16]));

        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 0 * 16]), d0);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 1 * 16]), d1);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 2 * 16]), d2);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 3 * 16]), d3);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 4 * 16]), d4);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 5 * 16]), d5);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 6 * 16]), d6);
        _mm_stream_si128(reinterpret_cast<__m128i*>(&dst[i + 7 * 16]), d7);
    }

    for(; i + 16 <= destination_size; i += 16)
    {
        // No need to set register attribute, because compile will interpret it as
        // CPU XMM register

        auto const d = _mm_load_si128(reinterpret_cast<__m128i *>(&src[i]));
        _mm_stream_si128(reinterpret_cast<__m128i *>(&dst[i]), d);
    }

    for(; i + 4 <= destination_size; i += 4)
    {
        *reinterpret_cast<uint32_t*>(&dst[i]) = 
            *reinterpret_cast<const uint32_t*>(&src[i]);
    }

    for(; i < destination_size; i++)
    {
        dst[i] = src[i];
    }

    _mm_sfence();
}

//------------------------------------------------------------------------------
/**
*/
void zero(void* destination, size_t const size_in_bytes)
{
    memset(destination, 0, size_in_bytes);
}

} // namespace xr::memory
//------------------------------------------------------------------------------