// This file is a part of xray-ng engine
//

#pragma once

//-----------------------------------------------------------------------------------------------------------

// XR_UNREFERENCED_PARAMETER macro
#if defined(XR_VA_HELPER_EXPAND)
#   error please do not define XR_VA_HELPER_EXPAND macros
#endif // #if defined(XR_VA_HELPER_EXPAND)

// helper macros for variadic macro overloading
#define XR_VA_HELPER_EXPAND(_X) _X  // workaround for Visual Studio

//-----------------------------------------------------------------------------------------------------------

// XR_VA_COUNT_HELPER macro
#if defined(XR_VA_COUNT_HELPER)
#   error please do not define XR_VA_COUNT_HELPER macros
#endif // #if defined(XR_VA_COUNT_HELPER)

#define XR_VA_COUNT_HELPER(_1, _2, _3, _4, _5, _6, _Count, ...) _Count

//-----------------------------------------------------------------------------------------------------------

// XR_VA_COUNT_HELPER macro
#if defined(XR_VA_COUNT)
#   error please do not define XR_VA_COUNT macros
#endif // #if defined(XR_VA_COUNT)

#define XR_VA_COUNT(...) XR_VA_HELPER_EXPAND(XR_VA_COUNT_HELPER(__VA_ARGS__, 6, 5, 4, 3, 2, 1))

//-----------------------------------------------------------------------------------------------------------

// XR_VA_SELECT_CAT macro
#if defined(XR_VA_SELECT_CAT)
#   error please do not define XR_VA_SELECT_CAT macros
#endif // #if defined(XR_VA_SELECT_CAT)

#define XR_VA_SELECT_CAT(_Name, _Count, ...) XR_VA_HELPER_EXPAND(_Name##_Count(__VA_ARGS__))

//-----------------------------------------------------------------------------------------------------------

// XR_VA_SELECT_HELPER macro
#if defined(XR_VA_SELECT_HELPER)
#   error please do not define XR_VA_SELECT_HELPER macros
#endif // #if defined(XR_VA_SELECT_HELPER)

#define XR_VA_SELECT_HELPER(_Name, _Count, ...) XR_VA_SELECT_CAT(_Name, _Count, __VA_ARGS__)

//-----------------------------------------------------------------------------------------------------------

// XR_VA_SELECT macro
#if defined(XR_VA_SELECT)
#   error please do not define XR_VA_SELECT macros
#endif // #if defined(XR_VA_SELECT)

#define XR_VA_SELECT(_Name, ...) XR_VA_SELECT_HELPER(_Name, XR_VA_COUNT(__VA_ARGS__), __VA_ARGS__)

//-----------------------------------------------------------------------------------------------------------