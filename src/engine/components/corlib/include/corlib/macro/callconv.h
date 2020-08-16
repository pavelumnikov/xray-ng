// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"

//-----------------------------------------------------------------------------------------------------------

// XR_VECTORIZED_CALL macro
#if defined(XR_VECTORIZED_CALL)
#   error please do not define XR_VECTORIZED_CALL macros
#endif // #if defined(XR_VECTORIZED_CALL)

#ifdef XR_MSVC_COMPILER_FAMILY
#   define XR_VECTORIZED_CALL __vectorcall
#else
#   define XR_VECTORIZED_CALL
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_FASTCALL_CONVENTION macro
#if defined(XR_FASTCALL_CONVENTION)
#   error please do not define XR_FASTCALL_CONVENTION macros
#endif // #if defined(XR_FASTCALL_CONVENTION)

#if XR_MSVC_COMPILER_FAMILY
#   define XR_FASTCALL_CONVENTION __fastcall
#elif XR_GCC_COMPILER_FAMILY
#   define XR_FASTCALL_CONVENTION __attribute__((fastcall))
#else
#   define XR_FASTCALL_CONVENTION
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_FASTCALL_CONVENTION macro
#if defined(XR_CDECL_CONVENTION)
#   error please do not define XR_CDECL_CONVENTION macros
#endif // #if defined(XR_CDECL_CONVENTION)

#if XR_MSVC_COMPILER_FAMILY
#   define XR_CDECL_CONVENTION __cdecl
#elif XR_GCC_COMPILER_FAMILY
#   define XR_CDECL_CONVENTION __attribute__((__cdecl__))
#else
#   define XR_CDECL_CONVENTION
#endif

//-----------------------------------------------------------------------------------------------------------