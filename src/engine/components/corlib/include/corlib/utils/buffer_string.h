// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/details/basic_buffer_string.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, utils)

using buffer_string = details::basic_buffer_string<char>;
using buffer_wstring = details::basic_buffer_string<wchar_t>;

XR_NAMESPACE_END(xr, utils)
//-----------------------------------------------------------------------------------------------------------
