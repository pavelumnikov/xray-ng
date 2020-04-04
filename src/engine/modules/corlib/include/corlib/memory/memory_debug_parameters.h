/*

  Copyright (c) 2019, Pavel Umnikov
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY
  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGE.

*/

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