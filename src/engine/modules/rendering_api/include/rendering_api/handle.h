// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
enum class handle_type : uint8_t
{
}; // enum class handle_type

//-----------------------------------------------------------------------------------------------------------
template<handle_type Type>
struct handle_wrapper
{
    static XR_CONSTEXPR_CPP14_OR_CONST uint16_t kInvalidHandle = UINT16_MAX;
    uint16_t idx;

    XR_CONSTEXPR_CPP14_OR_INLINE handle_wrapper() XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE bool is_valid() const XR_NOEXCEPT;
    XR_CONSTEXPR_CPP14_OR_INLINE handle_type type() const XR_NOEXCEPT;
};

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<handle_type Type>
XR_CONSTEXPR_CPP14_OR_INLINE handle_wrapper<Type>::handle_wrapper() XR_NOEXCEPT
    : idx(kInvalidHandle)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<handle_type Type>
XR_CONSTEXPR_CPP14_OR_INLINE bool handle_wrapper<Type>::is_valid() const XR_NOEXCEPT
{
    return idx != kInvalidHandle;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<handle_type Type>
XR_CONSTEXPR_CPP14_OR_INLINE handle_type handle_wrapper<Type>::type() const XR_NOEXCEPT
{
    return Type;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
template<handle_type Type>
XR_CONSTEXPR_CPP14_OR_INLINE bool is_valid(handle_wrapper<Type> _handle) XR_NOEXCEPT
{
    return _handle.is_valid();
}

//-----------------------------------------------------------------------------------------------------------

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------