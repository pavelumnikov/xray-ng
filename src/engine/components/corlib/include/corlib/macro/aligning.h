// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/assertion.h"

//-----------------------------------------------------------------------------------------------------------

// XR_ALIGN_TYPE macro
#if defined(XR_ALIGNAS)
#   error please do not define XR_ALIGNAS macros
#endif // #if defined(XR_ALIGNAS)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_ALIGNAS(_align) alignas(_align)
#else
#   if XR_GCC_COMPILER_FAMILY
#       define XR_ALIGNAS(_align) __attribute__((aligned((_align))))
#   elif XR_MSVC_COMPILER_FAMILY
#       define XR_ALIGNAS(_align) __declspec(align((_align)))
#   else
#       define XR_ALIGNAS(_align)
#   endif
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_MAX_CACHE_LINE_SIZE macro
#if defined(XR_MAX_CACHE_LINE_SIZE)
#   error please do not define XR_MAX_CACHE_LINE_SIZE macros
#endif // #if defined(XR_MAX_CACHE_LINE_SIZE)

// TODO: add for other platforms
#define XR_MAX_CACHE_LINE_SIZE 64

//-----------------------------------------------------------------------------------------------------------

// XR_DEFAULT_MACHINE_ALIGNMENT macro
#if defined(XR_DEFAULT_MACHINE_ALIGNMENT)
#   error please do not define XR_DEFAULT_MACHINE_ALIGNMENT macros
#endif // #if defined(XR_DEFAULT_MACHINE_ALIGNMENT)

// TODO: add for other platforms
#define XR_DEFAULT_MACHINE_ALIGNMENT 16

//-----------------------------------------------------------------------------------------------------------

// XR_ALIGN_DEFAULT_TYPE macro
#if defined(XR_ALIGN_DEFAULT_TYPE)
#   error please do not define XR_ALIGN_DEFAULT_TYPE macros
#endif // #if defined(XR_ALIGN_DEFAULT_TYPE)

#define XR_ALIGN_DEFAULT_TYPE(x) XR_ALIGNAS(XR_DEFAULT_MACHINE_ALIGNMENT)

//-----------------------------------------------------------------------------------------------------------

// XR_ALIGN_CACHE_LINE_TYPE macro
#if defined(XR_ALIGN_CACHE_LINE_TYPE)
#   error please do not define XR_ALIGN_CACHE_LINE_TYPE macros
#endif // #if defined(XR_ALIGN_DEFAULT_TYPE)

#define XR_ALIGN_CACHE_LINE_TYPE(x) XR_ALIGNAS(XR_MAX_CACHE_LINE_SIZE)

//-----------------------------------------------------------------------------------------------------------

// XR_ALIGN16_TYPE macro
#if defined(XR_ALIGN16_TYPE)
#   error please do not define XR_ALIGN16_TYPE macros
#endif // #if defined(XR_ALIGN16_TYPE)

#define XR_ALIGN16_TYPE(x) XR_ALIGNAS(16)

//-----------------------------------------------------------------------------------------------------------

// XR_ALIGN64_TYPE macro
#if defined(XR_ALIGN64_TYPE)
#   error please do not define XR_ALIGN64_TYPE macros
#endif // #if defined(XR_ALIGN64_TYPE)

#define XR_ALIGN64_TYPE(x) XR_ALIGNAS(64)

//-----------------------------------------------------------------------------------------------------------

// XR_ALIGN128_TYPE macro
#if defined(XR_ALIGN128_TYPE)
#   error please do not define XR_ALIGN128_TYPE macros
#endif // #if defined(XR_ALIGN128_TYPE)

#define XR_ALIGN128_TYPE(x) XR_ALIGNAS(128)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_2_BYTE_ALIGNED macro
#if defined(XR_ASSERT_2_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_2_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_2_BYTE_ALIGNED)

#define XR_ASSERT_2_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) &  1 ) == 0)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_4_BYTE_ALIGNED macro
#if defined(XR_ASSERT_4_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_4_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_4_BYTE_ALIGNED)

#define XR_ASSERT_4_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) &  3 ) == 0)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_8_BYTE_ALIGNED macro
#if defined(XR_ASSERT_8_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_8_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_8_BYTE_ALIGNED)

#define XR_ASSERT_8_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) &  7 ) == 0)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_16_BYTE_ALIGNED macro
#if defined(XR_ASSERT_16_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_16_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_16_BYTE_ALIGNED)

#define XR_ASSERT_16_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) & 15 ) == 0)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_32_BYTE_ALIGNED macro
#if defined(XR_ASSERT_32_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_32_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_32_BYTE_ALIGNED)

#define XR_ASSERT_32_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) & 31 ) == 0)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_64_BYTE_ALIGNED macro
#if defined(XR_ASSERT_64_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_64_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_64_BYTE_ALIGNED)

#define XR_ASSERT_64_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) & 63 ) == 0)

//-----------------------------------------------------------------------------------------------------------

// XR_ASSERT_128_BYTE_ALIGNED macro
#if defined(XR_ASSERT_128_BYTE_ALIGNED)
#   error please do not define XR_ASSERT_128_BYTE_ALIGNED macros
#endif // #if defined(XR_ASSERT_128_BYTE_ALIGNED)

#define XR_ASSERT_128_BYTE_ALIGNED( ptr ) XR_DEBUG_ASSERTION((((uintptr_t)(ptr)) & 127 ) == 0)

//-----------------------------------------------------------------------------------------------------------
