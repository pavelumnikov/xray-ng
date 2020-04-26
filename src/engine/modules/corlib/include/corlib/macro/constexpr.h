// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"

//-----------------------------------------------------------------------------------------------------------

// XR_CONSTEXPR_CPP14_OR_INLINE macro
#if defined(XR_CONSTEXPR_CPP14_OR_INLINE)
#   error please do not define XR_CONSTEXPR_CPP14_OR_INLINE macros
#endif // #if defined(XR_CONSTEXPR_CPP14_OR_INLINE)

#if XR_COMPILER_SUPPORTS_CPP14
#   define XR_CONSTEXPR_CPP14_OR_INLINE constexpr
#else
#   define XR_CONSTEXPR_CPP14_OR_INLINE inline
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_CONSTEXPR_CPP14_OR_CONST macro
#if defined(XR_CONSTEXPR_CPP14_OR_CONST)
#   error please do not define XR_CONSTEXPR_CPP14_OR_CONST macros
#endif // #if defined(XR_CONSTEXPR_CPP14_OR_CONST)

#if XR_COMPILER_SUPPORTS_CPP14
#   define XR_CONSTEXPR_CPP14_OR_CONST constexpr
#else
#   define XR_CONSTEXPR_CPP14_OR_CONST const
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_CONSTEXPR_OR_ENUM macro
#if defined(XR_CONSTEXPR_OR_ENUM)
#   error please do not define XR_CONSTEXPR_OR_ENUM macros
#endif // #if defined(XR_CONSTEXPR_OR_ENUM)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_CONSTEXPR_OR_ENUM(_type, _name, _value) static constexpr _type _name = (_value)
#else
#   define XR_CONSTEXPR_OR_ENUM(_type, _name, _value) enum { _name = (_type)((_value)) }
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_CONSTEXPR_IF macro
#if defined(XR_CONSTEXPR_IF)
#   error please do not define XR_CONSTEXPR_IF macros
#endif // #if defined(XR_CONSTEXPR_IF)

#if XR_COMPILER_SUPPORTS_CPP14
#   define XR_CONSTEXPR_IF(expr) if constexpr((expr))
#else
#   define XR_CONSTEXPR_IF(expr) if((expr))
#endif

//-----------------------------------------------------------------------------------------------------------