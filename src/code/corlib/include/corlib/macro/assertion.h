// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/string.h"
#include "corlib/macro/compiler.h"

#include <assert.h>

//-----------------------------------------------------------------------------------------------------------

// XR_DEBUG_ASSERT macro
#if defined(XR_DEBUG_ASSERTION) || defined(XR_DEBUG_ASSERTION_MSG)
#   error please do not define XR_DEBUG_ASSERTION or XR_DEBUG_ASSERTION_MSG macros
#endif // #if defined(XR_DEBUG_ASSERTION) || defined(XR_DEBUG_ASSERTION_MSG)

#define XR_DEBUG_ASSERTION(expr) assert(expr)
#define XR_DEBUG_ASSERTION_MSG(expr, msg) assert((expr) && msg)

//-----------------------------------------------------------------------------------------------------------

#if defined(XR_MSVC_COMPILER_FAMILY) && (_MSC_VER >= 1600) && defined(__cplusplus)
    // static_assert is defined by the compiler for both C and C++.
#elif defined(XR_CLANG_COMPILER_FAMILY) && defined(__cplusplus) 
#   if !(__has_feature(cxx_static_assert) || __has_extension(cxx_static_assert))
#      define __XR_NEED_CUSTOM_STATIC_ASSERT__
#   endif
#elif defined(XR_GCC_COMPILER_FAMILY) && (defined(__GXX_EXPERIMENTAL_CXX0X__) || (defined(__cplusplus) && (__cplusplus >= 201103L)))
    // static_assert is defined by the compiler.
#elif defined(__EDG__) && (defined(__cplusplus) && (__cplusplus >= 201103L))
    // static_assert is defined by the compiler.
#else
#   define __XR_NEED_CUSTOM_STATIC_ASSERT__
#endif

#ifdef __XR_NEED_CUSTOM_STATIC_ASSERT__
#if defined(__COUNTER__) // If this VC++ extension is available, which allows multiple statements per line...
#   define XR_STATIC_ASSERT(expression, description) \
    typedef char XR_STRING_CONCAT(compileTimeAssert,__COUNTER__) [((expression) != 0) ? 1 : -1]
#else
#   define XR_STATIC_ASSERT(expression, description) \
    typedef char XR_STRING_CONCAT(compileTimeAssert,__LINE__) [((expression) != 0) ? 1 : -1]
#endif
#undef NEED_CUSTOM_STATIC_ASSERT
#else
#   define XR_STATIC_ASSERT(desc, message) static_assert((desc), message)
#endif

//-----------------------------------------------------------------------------------------------------------