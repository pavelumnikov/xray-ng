// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math, details)

//-----------------------------------------------------------------------------------------------------------
//! A base random generator.
class base_random
{
protected:
    size_t get_prime(size_t seed);
}; // class base_random

XR_NAMESPACE_END(xr, math, details)
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, math)

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
}; // class fast_random<IntType>

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename IntType>
fast_random<IntType>::fast_random(size_t seed)
    : x(seed * get_prime(seed))
    , a(get_prime(x))
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename IntType>
IntType fast_random<IntType>::operator()()
{
    return get();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<typename IntType>
IntType fast_random<IntType>::get()
{
    return get(x);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint8_t
fast_random<uint8_t>::get(size_t& seed)
{
    uint8_t r = (uint8_t)(seed >> 8);
    seed = seed * a + 1;
    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint16_t
fast_random<uint16_t>::get(size_t& seed)
{
    uint32_t r = (uint32_t)(seed >> 16);
    seed = seed * a + 1;
    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint32_t
fast_random<uint32_t>::get(size_t& seed)
{
    uint32_t r = (uint32_t)(seed >> 32);
    seed = seed * a + 1;
    return r;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline uint64_t
fast_random<uint64_t>::get(size_t& seed)
{
    uint64_t r = (uint64_t)(seed);
    seed = seed * a + 1;
    return r;
}

XR_NAMESPACE_END(xr, math)
//-----------------------------------------------------------------------------------------------------------