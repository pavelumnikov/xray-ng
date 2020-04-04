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

#include "corlib/etl/select_sizet_constant.h" // for select_size_t_constant
#include "EASTL/numeric_limits.h"

//------------------------------------------------------------------------------
namespace xr::etl::algorithms
{

auto constexpr no_alignment_v = select_size_t_constant_v<1U, 1Ui64>;
auto constexpr default_alignment_v = select_size_t_constant_v<64U, 64Ui64>;

//------------------------------------------------------------------------------
/**
*/
template< typename T >
constexpr auto
align_up(T const arg, uintptr_t const alignment) noexcept
{
    if constexpr(eastl::is_pointer_v<T>)
        return T((reinterpret_cast<uintptr_t>(arg) + (alignment - 1)) & ~(alignment - 1));
    else
        return T((static_cast<uintptr_t>(arg) + (alignment - 1)) & ~(alignment - 1));
}

//------------------------------------------------------------------------------
/**
*/
template< typename T > // works for not power-of-2 alignments
constexpr auto
align_up_generic(T arg, uintptr_t const alignment) noexcept
{
    static_assert(eastl::is_scalar<T>::value);

    T mod = (arg % alignment);
    return !mod ? arg : arg + alignment - mod;
}

//------------------------------------------------------------------------------
/**
*/
template< typename T >
constexpr auto
align_down(T const arg, uintptr_t const alignment) noexcept
{
    if constexpr(::eastl::is_pointer<T>::value)
        return T(reinterpret_cast<uintptr_t>(arg)& ~(alignment - 1));
    else
        return T(static_cast<uintptr_t>(arg)& ~(alignment - 1));
}

//------------------------------------------------------------------------------
/**
*/
template<typename T>
constexpr bool
is_aligned(T* pointer, uintptr_t const alignment) noexcept
{
    return 0 == (reinterpret_cast<uintptr_t>(pointer) & (alignment - 1));
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
is_power_of_2(uintptr_t const arg) noexcept
{
    return arg && (0 == (arg & (arg - 1)));
}

//------------------------------------------------------------------------------
/**
*/
constexpr bool
is_power_of_2_multiple(uintptr_t const arg, uintptr_t const divisor) noexcept
{
    // Divisor is assumed to be a power of two (which is valid for current uses).
    //__IME_ASSERT_MSG(is_power_of_2(divisor), "Divisor should be a power of two");
    return arg && (arg & (divisor - 1));
}

//------------------------------------------------------------------------------
/**
*/
constexpr auto
next_highest_power_of_two(size_t v)
{
    // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    constexpr auto trip_count = eastl::numeric_limits<decltype(v)>::digits;
    v--;

    for(std::size_t i = 1; i < trip_count; i <<= 1)
    {
        v |= v >> i;
    }

    v++;
    return v;
}

} // namespace xr
//------------------------------------------------------------------------------