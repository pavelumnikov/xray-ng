// This file is a part of xray-ng engine
//

#pragma once

#include <stdint.h>
#include <stddef.h>

//-----------------------------------------------------------------------------------------------------------

// XR_CLANG_COMPILER_FAMILY macro
#if defined(XR_CLANG_COMPILER_FAMILY)
#   error please do not define XR_CLANG_COMPILER_FAMILY macros
#endif // #if defined(XR_CLANG_COMPILER_FAMILY)

// XR_GCC_COMPILER_FAMILY macro
#if defined(XR_GCC_COMPILER_FAMILY)
#   error please do not define XR_GCC_COMPILER_FAMILY macros
#endif // #if defined(XR_GCC_COMPILER_FAMILY)

// XR_MSVC_COMPILER_FAMILY macro
#if defined(XR_MSVC_COMPILER_FAMILY)
#   error please do not define XR_MSVC_COMPILER_FAMILY macros
#endif // #if defined(XR_MSVC_COMPILER_FAMILY)

#ifdef __clang__
#   define XR_CLANG_COMPILER_FAMILY (1)
#   define XR_GCC_COMPILER_FAMILY (1)
#elif __GNUC__
#   define XR_GCC_COMPILER_FAMILY (1)
#elif defined(_MSC_VER)
#   define XR_MSVC_COMPILER_FAMILY (1)
#elif defined(__ghs__) // for WiiU
#   define XR_GREEN_HILLS_COMPILER_FAMILY (1)
#   define XR_GCC_COMPILER_FAMILY (1)
#else
#   define XR_MSVC_COMPILER_FAMILY (0)
#   define XR_CLANG_COMPILER_FAMILY (0)
#   define XR_GCC_COMPILER_FAMILY (0)
#   define XR_GREEN_HILLS_COMPILER_FAMILY (0)
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_DEBUG macro
#if defined(XR_DEBUG)
#   error please do not define XR_DEBUG macros
#endif // #if defined(XR_DEBUG)

// XR_RELEASE macro
#if defined(XR_RELEASE)
#   error please do not define XR_RELEASE macros
#endif // #if defined(XR_RELEASE)

// XR_RELEASE macro
#if defined(MASTER_GOLD)
#   error please do not define MASTER_GOLD macros
#endif // #if defined(MASTER_GOLD)

#if defined(_DEBUG) || defined(DEBUG)
#   define XR_DEBUG 1
#elif defined(MASTER_GOLD)
#   define XR_MASTER_GOLD 1
#else
#   define XR_RELEASE 1
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_INSTRUMENTED_BUILD macro
#if defined(XR_INSTRUMENTED_BUILD)
#   error please do not define XR_INSTRUMENTED_BUILD macros
#endif // #if defined(XR_INSTRUMENTED_BUILD)

#if defined(XR_PROFILING)
#   define XR_INSTRUMENTED_BUILD (1)
#endif // defined(XR_PROFILING)

//-----------------------------------------------------------------------------------------------------------

// XR_COMPILER_SUPPORTS_CPP17 macro
#if defined(XR_COMPILER_SUPPORTS_CPP17)
#   error please do not define XR_COMPILER_SUPPORTS_CPP17 macros
#endif // #if defined(XR_COMPILER_SUPPORTS_CPP17)

#if XR_MSVC_COMPILER_FAMILY
#   if _MSVC_LANG >= 201703L
#      define XR_COMPILER_SUPPORTS_CPP17 1
#   endif // _MSVC_LANG >= 201703L
#endif // XR_MSVC_COMPILER_FAMILY

#ifndef XR_COMPILER_SUPPORTS_CPP17
#   define XR_COMPILER_SUPPORTS_CPP17 0
#endif // XR_COMPILER_SUPPORTS_CPP17

//-----------------------------------------------------------------------------------------------------------

// XR_COMPILER_SUPPORTS_CPP14 macro
#if defined(XR_COMPILER_SUPPORTS_CPP14)
#   error please do not define XR_COMPILER_SUPPORTS_CPP14 macros
#endif // #if defined(XR_COMPILER_SUPPORTS_CPP14)

#if XR_MSVC_COMPILER_FAMILY
#   if _MSVC_LANG >= 201402L
#      define XR_COMPILER_SUPPORTS_CPP14 1
#   endif // _MSVC_LANG >= 201402L
#endif // XR_MSVC_COMPILER_FAMILY

#ifndef XR_COMPILER_SUPPORTS_CPP14
#   define XR_COMPILER_SUPPORTS_CPP14 0
#endif // XR_COMPILER_SUPPORTS_CPP14

//-----------------------------------------------------------------------------------------------------------

// XR_COMPILER_SUPPORTS_CPP11 macro
#if defined(XR_COMPILER_SUPPORTS_CPP11)
#   error please do not define XR_COMPILER_SUPPORTS_CPP11 macros
#endif // #if defined(XR_COMPILER_SUPPORTS_CPP11)

#if XR_MSVC_COMPILER_FAMILY
#   if _MSVC_LANG >= 201402L
#      define XR_COMPILER_SUPPORTS_CPP11 1
#   endif // _MSVC_LANG >= 201402L
#endif // XR_MSVC_COMPILER_FAMILY

#ifndef XR_COMPILER_SUPPORTS_CPP11
#   define XR_COMPILER_SUPPORTS_CPP11 0
#endif // XR_COMPILER_SUPPORTS_CPP11

//-----------------------------------------------------------------------------------------------------------

// XR_COMPILER_ALLOCATOR_HINT macro
#if defined(XR_COMPILER_ALLOCATOR_HINT)
#   error please do not define XR_COMPILER_ALLOCATOR_HINT macros
#endif // #if defined(XR_COMPILER_ALLOCATOR_HINT)

#if XR_MSVC_COMPILER_FAMILY
#   if _MSVC_LANG >= 201402L
#      define XR_COMPILER_ALLOCATOR_HINT __declspec(allocator)
#   endif // _MSVC_LANG >= 201402L
#else
#   define XR_COMPILER_ALLOCATOR_HINT
#endif // XR_MSVC_COMPILER_FAMILY

//-----------------------------------------------------------------------------------------------------------