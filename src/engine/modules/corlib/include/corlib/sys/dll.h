// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"
#include "EASTL/string_view.h"

//-----------------------------------------------------------------------------------------------------------
namespace xr::sys
{

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid dll_load(eastl::string_view str);

//-----------------------------------------------------------------------------------------------------------
/**
*/
void dll_unload(pvoid library);

//-----------------------------------------------------------------------------------------------------------
/**
*/
pvoid dll_require_symbol(pvoid library, eastl::string_view str);

//-----------------------------------------------------------------------------------------------------------
/**
*/
template<typename Type>
Type dll_require_typed_symbol(pvoid library, eastl::string_view str)
{
    return reinterpret_cast<Type>(dll_require_symbol(library, str));
}

} // namespace xr::sys
//-----------------------------------------------------------------------------------------------------------
