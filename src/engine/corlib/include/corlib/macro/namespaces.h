// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/variadic.h"
#include "corlib/macro/compiler.h"

//-----------------------------------------------------------------------------------------------------------

// XR_NAMESPACE_BEGIN_HELPER1 macro
#if defined(XR_NAMESPACE_BEGIN_HELPER1)
#   error please do not define XR_NAMESPACE_BEGIN_HELPER1 macros
#endif // #if defined(XR_NAMESPACE_BEGIN_HELPER1)

// XR_NAMESPACE_BEGIN_HELPER2 macro
#if defined(XR_NAMESPACE_BEGIN_HELPER2)
#   error please do not define XR_NAMESPACE_BEGIN_HELPER2 macros
#endif // #if defined(XR_NAMESPACE_BEGIN_HELPER2)

// XR_NAMESPACE_BEGIN_HELPER3 macro
#if defined(XR_NAMESPACE_BEGIN_HELPER3)
#   error please do not define XR_NAMESPACE_BEGIN_HELPER3 macros
#endif // #if defined(XR_NAMESPACE_BEGIN_HELPER3)

// XR_NAMESPACE_BEGIN_HELPER4( macro
#if defined(XR_NAMESPACE_BEGIN_HELPER4)
#   error please do not define XR_NAMESPACE_BEGIN_HELPER4 macros
#endif // #if defined(XR_NAMESPACE_BEGIN_HELPER4)

// overloads for NAMESPACE_BEGIN
#if XR_COMPILER_SUPPORTS_CPP17
#   define XR_NAMESPACE_BEGIN_HELPER1(_Ns1) namespace _Ns1 {
#   define XR_NAMESPACE_BEGIN_HELPER2(_Ns1, _Ns2) namespace _Ns1::_Ns2 {
#   define XR_NAMESPACE_BEGIN_HELPER3(_Ns1, _Ns2, _Ns3) namespace _Ns1::_Ns2::_Ns3 {
#   define XR_NAMESPACE_BEGIN_HELPER4(_Ns1, _Ns2, _Ns3, _Ns4) namespace _Ns1::_Ns2::_Ns3::_Ns4 {
#else
#   define XR_NAMESPACE_BEGIN_HELPER1(_Ns1) namespace _Ns1 {
#   define XR_NAMESPACE_BEGIN_HELPER2(_Ns1, _Ns2) namespace _Ns1 { XR_NAMESPACE_BEGIN_HELPER1(_Ns2)
#   define XR_NAMESPACE_BEGIN_HELPER3(_Ns1, _Ns2, _Ns3) namespace _Ns1 { XR_NAMESPACE_BEGIN_HELPER2(_Ns2, _Ns3)
#   define XR_NAMESPACE_BEGIN_HELPER3(_Ns1, _Ns2, _Ns3, _Ns4) namespace _Ns1 { XR_NAMESPACE_BEGIN_HELPER3(_Ns2, _Ns3, _Ns4)
#endif

//-----------------------------------------------------------------------------------------------------------

// XR_NAMESPACE_BEGIN_HELPER1 macro
#if defined(XR_NAMESPACE_END_HELPER1)
#   error please do not define XR_NAMESPACE_END_HELPER1 macros
#endif // #if defined(XR_NAMESPACE_END_HELPER1)

// XR_NAMESPACE_END_HELPER2 macro
#if defined(XR_NAMESPACE_END_HELPER2)
#   error please do not define XR_NAMESPACE_END_HELPER2 macros
#endif // #if defined(XR_NAMESPACE_END_HELPER2)

// XR_NAMESPACE_BEGIN_HELPER3 macro
#if defined(XR_NAMESPACE_END_HELPER3)
#   error please do not define XR_NAMESPACE_END_HELPER3 macros
#endif // #if defined(XR_NAMESPACE_END_HELPER3)

// overloads for NAMESPACE_END
#if !XR_COMPILER_SUPPORTS_CPP17
#   define XR_NAMESPACE_END_HELPER1(_Ns1) }
#   define XR_NAMESPACE_END_HELPER2(_Ns1, _Ns2) } XR_NAMESPACE_END_HELPER1(_Ns2)
#   define XR_NAMESPACE_END_HELPER3(_Ns1, _Ns2, _Ns3) } XR_NAMESPACE_END_HELPER2(_Ns2, _Ns3)
#   define XR_NAMESPACE_END_HELPER3(_Ns1, _Ns2, _Ns3, _Ns4) } XR_NAMESPACE_END_HELPER3(_Ns2, _Ns3, _Ns4)
#endif // XR_COMPILER_SUPPORTS_CPP17

//-----------------------------------------------------------------------------------------------------------

// XR_NAMESPACE_BEGIN_HELPER3 macro
#if defined(XR_NAMESPACE_BEGIN)
#   error please do not define XR_NAMESPACE_BEGIN macros
#endif // #if defined(XR_NAMESPACE_BEGIN)

// final macros
#define XR_NAMESPACE_BEGIN(_Namespace, ...) XR_VA_SELECT(XR_NAMESPACE_BEGIN_HELPER, _Namespace, __VA_ARGS__)

// XR_NAMESPACE_END macro
#if defined(XR_NAMESPACE_END)
#   error please do not define XR_NAMESPACE_END macros
#endif // #if defined(XR_NAMESPACE_END)

#if XR_COMPILER_SUPPORTS_CPP17
#   define XR_NAMESPACE_END(_Namespace, ...) }
#else
#   define XR_NAMESPACE_END(_Namespace, ...) XR_VA_SELECT(XR_NAMESPACE_END_HELPER, _Namespace, __VA_ARGS__)
#endif // XR_COMPILER_SUPPORTS_CPP17

//-----------------------------------------------------------------------------------------------------------