// This file is a part of xray-ng engine
//

#pragma once

//-----------------------------------------------------------------------------------------------------------

// XR_STRING_CONCAT macro
#if defined(XR_STRING_CONCAT) || defined(XR_STRING_CONCAT_HELPER)
#   error please do not define XR_STRING_CONCAT or XR_STRING_CONCAT_HELPER macros
#endif // #if defined(XR_STRING_CONCAT) || defined(XR_STRING_CONCAT_HELPER)

#define XR_STRING_CONCAT_HELPER(a,b) a##b
#define XR_STRING_CONCAT(a,b) XR_STRING_CONCAT_HELPER(a,b)

//-----------------------------------------------------------------------------------------------------------

// XR_MAKE_STRING macro
#if defined(XR_MAKE_STRING) || defined(XR_MAKE_STRING_HELPER)
#   error please do not define XR_MAKE_STRING or XR_MAKE_STRING_HELPER macros
#endif // #if defined(XR_MAKE_STRING) || defined(XR_MAKE_STRING_HELPER)

#define XR_MAKE_STRING_HELPER(a) #a
#define XR_MAKE_STRING(a) XR_MAKE_STRING_HELPER(a)

//-----------------------------------------------------------------------------------------------------------