// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/select_sizet_constant.h" // for select_size_t_constant
#include "EASTL/numeric_limits.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

size_t XR_CONSTEXPR_CPP14_OR_CONST no_alignment_v = select_size_t_constant_v<1U, 1Ui64>;
size_t XR_CONSTEXPR_CPP14_OR_CONST default_alignment_v = select_size_t_constant_v<64U, 64Ui64>;

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T >
XR_CONSTEXPR_CPP14_OR_INLINE auto
align_up(T const arg, uintptr_t const alignment) XR_NOEXCEPT
{
    XR_CONSTEXPR_IF(eastl::is_pointer<T>::value)
        return T((reinterpret_cast<uintptr_t>(arg) + (alignment - 1)) & ~(alignment - 1));
    else
        return T((static_cast<uintptr_t>(arg) + (alignment - 1)) & ~(alignment - 1));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T > // works for not power-of-2 alignments
XR_CONSTEXPR_CPP14_OR_INLINE auto
align_up_generic(T arg, uintptr_t const alignment) XR_NOEXCEPT
{
    static_assert(eastl::is_scalar<T>::value);

    T mod = (arg % alignment);
    return !mod ? arg : arg + alignment - mod;
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template< typename T >
XR_CONSTEXPR_CPP14_OR_INLINE auto
align_down(T const arg, uintptr_t const alignment) XR_NOEXCEPT
{
    XR_CONSTEXPR_IF(::eastl::is_pointer<T>::value)
        return T(reinterpret_cast<uintptr_t>(arg)& ~(alignment - 1));
    else
        return T(static_cast<uintptr_t>(arg)& ~(alignment - 1));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename T>
XR_CONSTEXPR_CPP14_OR_INLINE bool
is_aligned(T* pointer, uintptr_t const alignment) XR_NOEXCEPT
{
    return 0 == (reinterpret_cast<uintptr_t>(pointer) & (alignment - 1));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE bool
is_power_of_2(uintptr_t const arg) XR_NOEXCEPT
{
    return arg && (0 == (arg & (arg - 1)));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE bool
is_power_of_2_multiple(uintptr_t const arg, uintptr_t const divisor) XR_NOEXCEPT
{
    // Divisor is assumed to be a power of two (which is valid for current uses).
    //__IME_ASSERT_MSG(is_power_of_2(divisor), "Divisor should be a power of two");
    return arg && (arg & (divisor - 1));
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
XR_CONSTEXPR_CPP14_OR_INLINE auto
next_highest_power_of_two(size_t v)
{
    // https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    XR_CONSTEXPR_CPP14_OR_CONST size_t trip_count = 64;
    v--;

    for(size_t i = 1; i < trip_count; i <<= 1)
        v |= v >> i;

    v++;
    return v;
}

} // namespace xr::utils
//-----------------------------------------------------------------------------------------------------------
