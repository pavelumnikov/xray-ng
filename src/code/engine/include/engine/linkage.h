// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"

#ifndef XR_IO_API // If the build file hasn't already defined this to be dllexport...
#   ifdef XR_STATIC_LIBRARIES
#       define XR_IO_API
#   else // #ifdef XR_STATIC_LIBRARIES
#       ifndef XR_IO_BUILDING
#           define XR_IO_API XR_COMPILER_DLL_IMPORT
#       else // #ifndef XR_IO_BUILDING
#           define XR_IO_API XR_COMPILER_DLL_EXPORT
#       endif // #ifndef XR_IO_BUILDING
#   endif // #ifdef XR_STATIC_LIBRARIES
#endif