// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"

//-----------------------------------------------------------------------------------------------------------

#if XR_COMPILER_SUPPORTS_CPP17
namespace xr::details
{
template<typename ... Args>
void unreferenced_arguments(Args&&... args)
{}
} // namespace xr::details
#else
namespace xr
{
namespace details
{
inline void unreferenced_arguments(...)
{}
} // namespace details
} // namespace xr
#endif // XR_COMPILER_SUPPORTS_CPP17

//-----------------------------------------------------------------------------------------------------------

// XR_UNREFERENCED_PARAMETER macro
#if defined(XR_UNREFERENCED_PARAMETER)
#   error please do not define XR_UNREFERENCED_PARAMETER macros
#endif // #if defined(XR_UNREFERENCED_PARAMETER)

#define XR_UNREFERENCED_PARAMETER(...) (xr::details::unreferenced_arguments(__VA_ARGS__))

//-----------------------------------------------------------------------------------------------------------