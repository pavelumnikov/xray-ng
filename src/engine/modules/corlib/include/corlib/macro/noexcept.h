// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/macro/compiler.h"

//-----------------------------------------------------------------------------------------------------------

#ifdef XR_NOEXCEPT
#   error do not define XR_NOEXCEPT macro
#endif // #ifdef XR_NOEXCEPT

#if XR_COMPILER_SUPPORTS_CPP11
#   define XR_NOEXCEPT noexcept(true)
#else
#   define XR_NOEXCEPT throw()
#endif

//-----------------------------------------------------------------------------------------------------------