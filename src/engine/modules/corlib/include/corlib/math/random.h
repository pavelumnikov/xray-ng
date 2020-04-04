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

#include "corlib/types.h"

namespace xr::math::details
{

//! A base random generator.
class base_random
{
protected:
    size_t get_prime(size_t seed);
};

} // namespace xr::math::details

namespace xr::math
{

//! A fast random number generator.
/** Uses linear congruential method. */
template<typename IntType>
class fast_random : public details::base_random
{
public:
    //! Construct a random number generator.
    fast_random(size_t seed);

    //! Get a random number.
    IntType get();

    //! Get a random number as functor.
    IntType operator()();

private:
    //! Get a random number for the given seed; update the seed for next use.
    IntType get(size_t& seed);
    size_t x;
    size_t a;
};

template<typename IntType>
fast_random<IntType>::fast_random(size_t seed)
    : x { seed * get_prime(seed) }
    , a { get_prime(x) }
{}

template<typename IntType>
IntType fast_random<IntType>::operator()()
{
    return get();
}

template<typename IntType>
IntType fast_random<IntType>::get()
{
    return get(x);
}

inline uint8_t
fast_random<uint8_t>::get(size_t& seed)
{
    uint8_t r = (uint8_t)(seed >> 8);
    seed = seed * a + 1;
    return r;
}

inline uint16_t
fast_random<uint16_t>::get(size_t& seed)
{
    uint32_t r = (uint32_t)(seed >> 16);
    seed = seed * a + 1;
    return r;
}

inline uint32_t
fast_random<uint32_t>::get(size_t& seed)
{
    uint32_t r = (uint32_t)(seed >> 32);
    seed = seed * a + 1;
    return r;
}

inline uint64_t
fast_random<uint64_t>::get(size_t& seed)
{
    uint64_t r = (uint64_t)(seed);
    seed = seed * a + 1;
    return r;
}

} // namespace xr::math