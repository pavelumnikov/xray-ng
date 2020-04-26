// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/string_view.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sys)

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid dll_load(utils::string_view str);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void dll_unload(pvoid library);

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid dll_require_symbol(pvoid library, utils::string_view str);

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
Type dll_require_typed_symbol(pvoid library, utils::string_view str)
{
    return reinterpret_cast<Type>(dll_require_symbol(library, str));
}

XR_NAMESPACE_END(xr, sys)
//-----------------------------------------------------------------------------------------------------------
