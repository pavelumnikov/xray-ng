// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"

//-----------------------------------------------------------------------------------------------------------

// XR_NON_VIRTUAL macro
#if defined(XR_NON_VIRTUAL)
#   error please do not define XR_NON_VIRTUAL macros
#endif // #if defined(XR_NON_VIRTUAL)

#if XR_MSVC_COMPILER_FAMILY
#   define XR_NON_VIRTUAL __declspec(novtable)
#else
#   define XR_NON_VIRTUAL
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_FORWARD_DECLARE_INTERFACE macro
#if defined(XR_FORWARD_DECLARE_INTERFACE)
#   error please do not define XR_FORWARD_DECLARE_INTERFACE macros
#endif // #if defined(XR_FORWARD_DECLARE_INTERFACE)

#ifdef XR_MSVC_COMPILER_FAMILY
#   define XR_FORWARD_DECLARE_INTERFACE(x) __interface x
#else
#   define XR_FORWARD_DECLARE_INTERFACE(x) struct x
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_INTERFACE macro
#if defined(XR_DECLARE_INTERFACE)
#   error please do not define XR_DECLARE_INTERFACE macros
#endif // #if defined(XR_DECLARE_INTERFACE)

#define XR_DECLARE_INTERFACE(x) struct XR_NON_VIRTUAL x

//-----------------------------------------------------------------------------------------------------------

// XR_PURE_VIRTUAL_DESTRUCTOR macro
#if defined(XR_PURE_VIRTUAL_DESTRUCTOR)
#   error please do not define XR_PURE_VIRTUAL_DESTRUCTOR macros
#endif // #if defined(XR_PURE_VIRTUAL_DESTRUCTOR)

#if XR_MSVC_COMPILER_FAMILY
#    define XR_PURE_VIRTUAL_DESTRUCTOR(x) virtual ~x() = 0 {}
#else
#    define XR_PURE_VIRTUAL_DESTRUCTOR(x) virtual ~x() = 0
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_DESTRUCTOR macro
#if defined(XR_DECLARE_DEFAULT_DESTRUCTOR)
#   error please do not define XR_DECLARE_DEFAULT_DESTRUCTOR macros
#endif // #if defined(XR_DECLARE_DEFAULT_DESTRUCTOR)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_DECLARE_DEFAULT_DESTRUCTOR(x) public: ~x() = default
#else
#   define XR_DECLARE_DEFAULT_DESTRUCTOR(x) public: ~x() {}
#endif // #if XR_COMPILER_SUPPORTS_CPP11

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT macro
#if defined(XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT)
#   error please do not define XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT macros
#endif // #if defined(XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT)

#define XR_DECLARE_DEFAULT_MOVE_ASSIGNMENT(x) \
 public: \
     x(x&&) = default; \
     x& operator=(x&&) = default

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT macro
#if defined(XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT)
#   error please do not define XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT macros
#endif // #if defined(XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT)

#define XR_DECLARE_DEFAULT_CONSTEXPR_MOVE_ASSIGNMENT(x) \
 public: \
     constexpr x(x&&) = default; \
     x& operator=(x&&) = default

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_COPY_ASSIGNMENT macro
#if defined(XR_DECLARE_DEFAULT_COPY_ASSIGNMENT)
#   error please do not define XR_DECLARE_DEFAULT_COPY_ASSIGNMENT macros
#endif // #if defined(XR_DECLARE_DEFAULT_COPY_ASSIGNMENT)

#define XR_DECLARE_DEFAULT_COPY_ASSIGNMENT(x) \
 public: \
     x(x const&) = default; \
     x& operator=(x const&) = default

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_CONSTEXPR_COPY_ASSIGNMENT macro
#if defined(XR_DECLARE_DEFAULT_SAFE_COPY_ASSIGNMENT)
#   error please do not define XR_DECLARE_DEFAULT_SAFE_COPY_ASSIGNMENT macros
#endif // #if defined(XR_DECLARE_DEFAULT_SAFE_COPY_ASSIGNMENT)

#define XR_DECLARE_DEFAULT_SAFE_COPY_ASSIGNMENT(x) \
 public: \
     x(x const&) noexcept = default; \
     x& operator=(x const&) noexcept = default

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DELETE_COPY_ASSIGNMENT macro
#if defined(XR_DECLARE_DELETE_COPY_ASSIGNMENT)
#   error please do not define XR_DECLARE_DELETE_COPY_ASSIGNMENT macros
#endif // #if defined(XR_DECLARE_DELETE_COPY_ASSIGNMENT)

#define XR_DECLARE_DELETE_COPY_ASSIGNMENT(x) \
 public: \
     x(x const&) = delete; \
     x& operator=(x const&) = delete

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_DESTRUCTOR macro
#if defined(XR_DECLARE_DELETE_MOVE_ASSIGNMENT)
#   error please do not define XR_DECLARE_DELETE_MOVE_ASSIGNMENT macros
#endif // #if defined(XR_DECLARE_DELETE_MOVE_ASSIGNMENT)

#define XR_DECLARE_DELETE_MOVE_ASSIGNMENT(x) \
 public: \
     x(x&&) = delete; \
     x& operator=(x&&) = delete

//-----------------------------------------------------------------------------------------------------------