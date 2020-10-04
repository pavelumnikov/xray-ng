// This file is a part of xray-ng engine
//

#include "corlib/macro/constexpr.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, constants)

//-----------------------------------------------------------------------------------------------------------
// shared

XR_CONSTEXPR_CPP14_OR_STATIC_CONST char xrayng_default_config[] = "config";
XR_CONSTEXPR_CPP14_OR_STATIC_CONST char xrayng_caption[] = "xray-ng";

//-----------------------------------------------------------------------------------------------------------
// messages

XR_CONSTEXPR_CPP14_OR_STATIC_CONST char vulkan_support_error_msg[] = "Vulkan is not supported";

//-----------------------------------------------------------------------------------------------------------
// configuration file

XR_CONSTEXPR_CPP14_OR_STATIC_CONST char window_width[] = "r_window_width";
XR_CONSTEXPR_CPP14_OR_STATIC_CONST char window_height[] = "r_window_height";

XR_NAMESPACE_END(xr, constants)
//-----------------------------------------------------------------------------------------------------------