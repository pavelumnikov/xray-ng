// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"
#include "corlib/macro/constexpr.h"

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

#define XR_FORWARD_DECLARE_INTERFACE(x) struct x

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_INTERFACE macro
#if defined(XR_DECLARE_INTERFACE)
#   error please do not define XR_DECLARE_INTERFACE macros
#endif // #if defined(XR_DECLARE_INTERFACE)

#define XR_DECLARE_INTERFACE(x) struct XR_NON_VIRTUAL x

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_INTERFACE macro
#if defined(XR_DECLARE_SINGLE_INHERITED_INTERFACE)
#   error please do not define XR_DECLARE_SINGLE_INHERITED_INTERFACE macros
#endif // #if defined(XR_DECLARE_SINGLE_INHERITED_INTERFACE)

#define XR_DECLARE_SINGLE_INHERITED_INTERFACE(x, y) struct XR_NON_VIRTUAL x : public y

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

// XR_DECLARE_DEFAULT_CONSTRUCTOR macro
#if defined(XR_DECLARE_DEFAULT_CONSTRUCTOR)
#   error please do not define XR_DECLARE_DEFAULT_CONSTRUCTOR macros
#endif // #if defined(XR_DECLARE_DEFAULT_CONSTRUCTOR)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_DECLARE_DEFAULT_CONSTRUCTOR(x) public: x() = default
#else
#   define XR_DECLARE_DEFAULT_CONSTRUCTOR(x) public: x() {}
#endif // #if XR_DECLARE_DEFAULT_CONSTRUCTOR

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_CONSTRUCTOR macro
#if defined(XR_DECLARE_DEFAULT_NOEXCEPT_CONSTRUCTOR)
#   error please do not define XR_DECLARE_DEFAULT_NOEXCEPT_CONSTRUCTOR macros
#endif // #if defined(XR_DECLARE_DEFAULT_NOEXCEPT_CONSTRUCTOR)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_DECLARE_DEFAULT_NOEXCEPT_CONSTRUCTOR(x) public: x() XR_NOEXCEPT = default
#else
#   define XR_DECLARE_DEFAULT_NOEXCEPT_CONSTRUCTOR(x) public: x() XR_NOEXCEPT {}
#endif // #if XR_DECLARE_DEFAULT_CONSTRUCTOR

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_CONSTRUCTOR macro
#if defined(XR_DECLARE_DEFAULT_CONSTEXPR_NOEXCEPT_CONSTRUCTOR)
#   error please do not define XR_DECLARE_DEFAULT_CONSTEXPR_NOEXCEPT_CONSTRUCTOR macros
#endif // #if defined(XR_DECLARE_DEFAULT_CONSTEXPR_NOEXCEPT_CONSTRUCTOR)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_DECLARE_DEFAULT_CONSTEXPR_NOEXCEPT_CONSTRUCTOR(x) public: XR_CONSTEXPR_CPP14_OR_INLINE x() XR_NOEXCEPT = default
#else
#   define XR_DECLARE_DEFAULT_CONSTEXPR_NOEXCEPT_CONSTRUCTOR(x) public: XR_CONSTEXPR_CPP14_OR_INLINE x() XR_NOEXCEPT {}
#endif // #if XR_DECLARE_DEFAULT_CONSTRUCTOR

//-----------------------------------------------------------------------------------------------------------

// XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC macro
#if defined(XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC)
#   error please do not define XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC macros
#endif // #if defined(XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC)

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC(x) x() = default
#else
#   define XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC(x) x() {}
#endif // #if XR_DECLARE_DEFAULT_CONSTRUCTOR_NO_PUBLIC

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