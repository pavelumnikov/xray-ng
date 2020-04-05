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

#include <cstdint>
#include <cstddef>
#include <cassert>

namespace xr::details
{
template<typename ... Args>
void unreferenced_arguments(Args... args)
{}

#if defined(__GNUC__)
static inline bool (likely)(bool x)
{
    return __builtin_expect((x), true);
}
static inline bool (unlikely)(bool x)
{
    return __builtin_expect((x), false);
}
#else
static inline bool (likely)(bool x)
{
    return x;
}
static inline bool (unlikely)(bool x)
{
    return x;
}
#endif
} // namespace xr::details

//
// Define some things for platform and include needed headers
//

// Compiler family
////////////////////////////////////////////////////////////////////////

#ifdef __clang__
#   define XR_CLANG_COMPILER_FAMILY (1)
#   define XR_GCC_COMPILER_FAMILY (1)
#elif __GNUC__
#   define XR_GCC_COMPILER_FAMILY (1)
#elif defined(_MSC_VER)
#   define XR_MSVC_COMPILER_FAMILY (1)
#else
#   define XR_MSVC_COMPILER_FAMILY (0)
#   define XR_CLANG_COMPILER_FAMILY (0)
#   define XR_GCC_COMPILER_FAMILY (0)
#endif

// Debug / Release
////////////////////////////////////////////////////////////////////////

#if defined(_DEBUG) || defined(DEBUG)
#    define XR_DEBUG (1)
#else
#    define XR_RELEASE (1)
#endif

// Profiling
////////////////////////////////////////////////////////////////////////

#if defined(XR_PROFILING)
#   define XR_INSTRUMENTED_BUILD (1)
#endif // defined(XR_PROFILING)

//------------------------------------------------------------------------------

#ifndef XR_UNREFERENCED_PARAMETER
#   define XR_UNREFERENCED_PARAMETER(...) (xr::details::unreferenced_arguments(__VA_ARGS__))
#endif // #ifndef XR_UNREFERENCED_PARAMETER

//------------------------------------------------------------------------------

#ifndef XR_UNLIKELY
#   define XR_UNLIKELY(x) xr::details::unlikely((x))
#endif // #ifndef XR_UNLIKELY

//------------------------------------------------------------------------------

#ifndef XR_LIKELY
#   define XR_LIKELY(x) xr::details::likely((x))
#endif // #ifndef XR_LIKELY

//------------------------------------------------------------------------------

#ifndef XR_VECTORIZED_CALL
#   ifdef XR_MSVC_COMPILER_FAMILY
#       define XR_VECTORIZED_CALL __vectorcall
#   else
#       define XR_VECTORIZED_CALL
#   endif
#endif // #ifndef XR_VECTORIZED_CALL

//------------------------------------------------------------------------------

#ifndef XR_FASTCALL_CONVENTION
#   if XR_MSVC_COMPILER_FAMILY
#       define XR_FASTCALL_CONVENTION __fastcall
#   elif XR_GCC_COMPILER_FAMILY
#       define XR_FASTCALL_CONVENTION __attribute__((fastcall))
#   else
#       define XR_FASTCALL_CONVENTION
#   endif
#endif // #ifndef XR_FASTCALL_CONVENTION

//------------------------------------------------------------------------------

#ifndef XR_CDECL_CONVENTION
#   if XR_MSVC_COMPILER_FAMILY
#       define XR_CDECL_CONVENTION __cdecl
#   elif XR_GCC_COMPILER_FAMILY
#       define XR_CDECL_CONVENTION __attribute__((__cdecl__))
#   else
#       define XR_CDECL_CONVENTION
#   endif
#endif // #ifndef XR_CDECL_CONVENTION

//------------------------------------------------------------------------------

#ifndef XR_MAX_CACHE_LINE_SIZE
#   define XR_MAX_CACHE_LINE_SIZE 64
#endif // #ifndef XR_MAX_CACHE_LINE_SIZE

//------------------------------------------------------------------------------

#ifndef XR_DEFAULT_MACHINE_ALIGNMENT
#   define XR_DEFAULT_MACHINE_ALIGNMENT 16
#endif // #ifndef XR_DEFAULT_MACHINE_ALIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_NON_VIRTUAL
#   if XR_MSVC_COMPILER_FAMILY
#       define XR_NON_VIRTUAL __declspec(novtable)
#   else
#       define XR_NON_VIRTUAL
#   endif
#endif // #ifndef XR_NON_VIRTUAL

//------------------------------------------------------------------------------

#ifndef XR_FORWARD_DECLARE_INTERFACE
#   ifdef XR_MSVC_COMPILER_FAMILY
#       define XR_FORWARD_DECLARE_INTERFACE(x) __interface x
#   else
#       define XR_FORWARD_DECLARE_INTERFACE(x) struct x
#   endif
#endif // #ifndef XR_DECLARE_INTERFACE

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_INTERFACE
#   ifdef XR_MSVC_COMPILER_FAMILY
#       define XR_DECLARE_INTERFACE(x) __interface x
#   else
#       define XR_DECLARE_INTERFACE(x) struct XR_NON_VIRTUAL x
#   endif
#endif // #ifndef XR_DECLARE_INTERFACE

//------------------------------------------------------------------------------

#ifndef XR_PURE_VIRTUAL_DESTRUCTOR
#   if XR_MSVC_COMPILER_FAMILY
#       define XR_PURE_VIRTUAL_DESTRUCTOR(x) virtual ~x() = 0 {}
#   else
#       define XR_PURE_VIRTUAL_DESTRUCTOR(x) virtual ~x() = 0
#   endif
#endif // #ifndef XR_PURE_VIRTUAL_DESTRUCTOR

//------------------------------------------------------------------------------

#ifndef XR_PURE_VIRTUAL_CLASS
#   define XR_PURE_VIRTUAL_CLASS(x) \
    protected: \
        x() noexcept = default; \
    public: \
        XR_PURE_VIRTUAL_DESTRUCTOR(x); \
        x(x const&) = default; \
        x(x&&) = default; \
        x& operator=(x const&) = default; \
        x& operator=(x&&) = default;

#endif // #ifndef XR_PURE_VIRTUAL_CLASS

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_DESTRUCTOR
#   define XR_DECLARE_DEFAULT_DESTRUCTOR(x) \
    public: \
        ~x() = default
#endif // #ifndef XR_DECLARE_DEFAULT_DESTRUCTOR

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_COPY_ASSIGNMENT
#   define XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(x) \
    public: \
        x(x const&) = default; \
        x& operator=(x const&) = default
#endif // #ifndef XR_DECLARE_DEFAULT_COPY_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT
#   define XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT(x) \
    public: \
        constexpr x(x const&) noexcept = default; \
        x& operator=(x const&) noexcept = default
#endif // #ifndef XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DELETE_COPY_ASSIGNMENT
#   define XR_DECLARE_DELETE_COPY_ASSIGNMENT(x) \
    public: \
        x(x const&) = delete; \
        x& operator=(x const&) = delete
#endif // #ifndef XR_DECLARE_DELETE_COPY_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT
#   define XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(x) \
    public: \
        x(x&&) = default; \
        x& operator=(x&&) = default
#endif // #ifndef XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT
#   define XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT(x) \
    public: \
        constexpr x(x&&) noexcept = default; \
        x& operator=(x&&) noexcept = default
#endif // #ifndef XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_SAFE_MOVE_ASSIGNMENT
#   define XR_DECLARE_DEFAULT_SAFE_MOVE_ASSIGNMENT(x) \
    public: \
        x(x&&) noexcept = default; \
        x& operator=(x&&) noexcept = default
#endif // #ifndef XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DELETE_MOVE_ASSIGNMENT
#   define XR_DECLARE_DELETE_MOVE_ASSIGNMENT(x) \
    public: \
        x(x&&) = delete; \
        x& operator=(x&&) = delete
#endif // #ifndef XR_DECLARE_DELETE_MOVE_ASSIGNMENT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT
#   define XR_DECLARE_DEFAULT(x)  \
        XR_DECLARE_DEFAULT_DESTRUCTOR(x); \
        XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(x); \
        XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(x)
#endif // #ifndef XR_DECLARE_DEFAULT

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_CONSTEXPR_CLASS
#   define XR_DECLARE_DEFAULT_CONSTEXPR_CLASS(x)  \
        constexpr x() noexcept = default; \
        XR_DECLARE_DEFAULT_DESTRUCTOR(x); \
        XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT(x); \
        XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT(x)
#endif // #ifndef XR_DECLARE_DEFAULT_CONSTEXPR_CLASS

//------------------------------------------------------------------------------

#ifndef XR_DECLARE_DEFAULT_MOVABLE_ONLY_CONSTEXPR_CLASS
#   define XR_DECLARE_DEFAULT_MOVABLE_ONLY_CONSTEXPR_CLASS(x)  \
        constexpr x() noexcept = default; \
        XR_DECLARE_DEFAULT_DESTRUCTOR(x); \
        XR_DECLARE_DELETE_COPY_ASSIGNMENT(x); \
        XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT(x)
#endif // #ifndef XR_DECLARE_DEFAULT_MOVABLE_ONLY_CONSTEXPR_CLASS

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

#ifndef XR_DISABLE_DIRECT_NEW_CALL
#   define XR_DISABLE_DIRECT_NEW_CALL \
    public: \
        void* operator new(size_t) = delete; \
        void operator delete(void*) = delete
#endif // #ifndef XR_DECLARE_DELETE_MOVE_ASSIGNMENT

//------------------------------------------------------------------------------

// XR_ALIGNAS macro
#if defined(XR_ALIGNAS)
#   error please do not define XR_ALIGNAS macros
#endif // #if defined(XR_ALIGNAS)

#define XR_ALIGNAS(x) alignas((x))

//------------------------------------------------------------------------------

// XR_STRING_CONCAT macro
#if defined(XR_STRING_CONCAT) || defined(XR_STRING_CONCAT_HELPER)
#   error please do not define XR_STRING_CONCAT or XR_STRING_CONCAT_HELPER macros
#endif // #if defined(XR_STRING_CONCAT) || defined(XR_STRING_CONCAT_HELPER)

#define XR_STRING_CONCAT_HELPER(a,b) a##b
#define XR_STRING_CONCAT(a,b) XR_STRING_CONCAT_HELPER(a,b)

//------------------------------------------------------------------------------

// XR_MAKE_STRING macro
#if defined(XR_MAKE_STRING) || defined(XR_MAKE_STRING_HELPER)
#   error please do not define XR_MAKE_STRING or XR_MAKE_STRING_HELPER macros
#endif // #if defined(XR_MAKE_STRING) || defined(XR_MAKE_STRING_HELPER)

#define XR_MAKE_STRING_HELPER(a) #a
#define XR_MAKE_STRING(a) XR_MAKE_STRING_HELPER(a)

//------------------------------------------------------------------------------

// XR_DEBUG_ASSERT macro
#if defined(XR_DEBUG_ASSERTION) || defined(XR_DEBUG_ASSERTION_MSG)
#   error please do not define XR_DEBUG_ASSERTION or XR_DEBUG_ASSERTION_MSG macros
#endif // #if defined(XR_DEBUG_ASSERTION) || defined(XR_DEBUG_ASSERTION_MSG)

#define XR_DEBUG_ASSERTION(expr) assert(expr)
#define XR_DEBUG_ASSERTION_MSG(expr, msg) assert((expr) && msg)

//------------------------------------------------------------------------------

#ifdef XR_TODO_COMMENT
#   error do not define XR_TODO_COMMENT macro
#endif // #ifdef XR_TODO_COMMENT

#define XR_TODO_COMMENT(message_to_show) \
    __pragma(message(__FILE__ "(" XR_MAKE_STRING(__LINE__) "): @todo: " message_to_show))

//------------------------------------------------------------------------------

#if defined(XR_MSVC_COMPILER_FAMILY) && (_MSC_VER >= 1913)  // VS2017+
#   define XR_HAS_UNIQUE_OBJECT_REPRESENTATIONS_AVAILABLE 1
#elif defined(XR_CLANG_COMPILER_FAMILY)
#   if !__is_identifier(__has_unique_object_representations)
#       define XR_HAS_UNIQUE_OBJECT_REPRESENTATIONS_AVAILABLE 1
#   else
#       define XR_HAS_UNIQUE_OBJECT_REPRESENTATIONS_AVAILABLE 0
#   endif
#else
#   define XR_HAS_UNIQUE_OBJECT_REPRESENTATIONS_AVAILABLE 0
#endif

//------------------------------------------------------------------------------

//
// Code analysis
//

//------------------------------------------------------------------------------

#if XR_MSVC_COMPILER_FAMILY && defined(XRAY_PLATFORM_WINDOWS)
#   include <sal.h>
#endif // #if _MSC_VER && defined(XRAY_PLATFORM_WINDOWS)

//------------------------------------------------------------------------------

#ifndef XR_VERIFY_FORMAT_STRING
#	define XR_VERIFY_FORMAT_STRING
#endif // #ifndef XR_VERIFY_FORMAT_STRING

//------------------------------------------------------------------------------

#ifndef XR_GCC_VERIFY_FORMAT_STRING
#	define XR_GCC_VERIFY_FORMAT_STRING
#endif // #ifndef XR_GCC_VERIFY_FORMAT_STRING

//------------------------------------------------------------------------------

#ifndef ParamNotNull
#	define ParamNotNull _In_
#endif // ParamNotNull

//------------------------------------------------------------------------------

#ifndef ParamCanBeNull
#	define ParamCanBeNull _In_opt_
#endif // ParamCanBeNull

//------------------------------------------------------------------------------

#ifndef ReturnNotNull
#	define ReturnNotNull _Ret_notnull_
#endif // ReturnNotNull

//------------------------------------------------------------------------------

#ifndef ReturnCanBeNull
#	define ReturnCanBeNull _Ret_maybenull_
#endif // ReturnCanBeNull

//------------------------------------------------------------------------------

#ifndef ReturnWritableBlockOfBytes
#	define ReturnWritableBlockOfBytes(bytes) _Post_writable_byte_size_(bytes)
#endif // ReturnWritableBlockOfBytes

//------------------------------------------------------------------------------