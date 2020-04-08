// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

#if defined(XR_DEBUG)
#	define XR_DEBUG_PARAMETERS_DECLARATION_NO_COMA		char const* function, char const* file, uint32_t const line
#	define XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD		XR_UNREFERENCED_PARAMETER(function, file, line);

#	define XR_DEBUG_PARAMETERS_DECLARATION				, XR_DEBUG_PARAMETERS_DECLARATION_NO_COMA
#	define XR_DEBUG_PARAMETERS							, function, file, line
#	define XR_DEBUG_PARAMETERS_TYPEID(T)				, typeid(T).name()
#	define XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION	, char const* const description
#	define XR_DEBUG_PARAMETERS_DESCRIPTION				, description
#	define XR_DEBUG_PARAMETERS_DESCRIPTION_PARAMETER	description

#   define XR_DEBUG_PARAMETERS_DESCRIPTION_DEFINITION(m) , m
#   define XR_DEBUG_PARAMETERS_DEFINITION               , __FUNCTION__, __FILE__, __LINE__

#else // #ifdef DEBUG
#	define XR_DEBUG_PARAMETERS_DECLARATION_NO_COMA	
#	define XR_DEBUG_PARAMETERS_UNREFERENCED_GUARD		
#	define XR_DEBUG_PARAMETERS_DECLARATION
#	define XR_DEBUG_PARAMETERS
#	define XR_DEBUG_PARAMETERS_TYPEID(T)
#	define XR_DEBUG_PARAMETERS_DESCRIPTION_DECLARATION
#	define XR_DEBUG_PARAMETERS_DESCRIPTION
#	define XR_DEBUG_PARAMETERS_DESCRIPTION_PARAMETER		""

#endif // #ifdef DEBUG