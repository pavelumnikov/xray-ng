///////////////////////////////////////////////////////////////////////////////
// EASprintf.h
//
// Copyright (c) Electronic Arts. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef EASTDC_EASPRINTF_H
#define EASTDC_EASPRINTF_H

#include <EASTL/EABase/eabase.h>
#include <EASTL/EABase/eastdarg.h>
#include <stdio.h>

#define EASTDC_API

namespace EA {
namespace StdC {

EASTDC_API int Vsnprintf(char* EA_RESTRICT pDestination, size_t n, const char* EA_RESTRICT pFormat, va_list arguments);
EASTDC_API int Vsnprintf(char16_t* EA_RESTRICT pDestination, size_t n, const char16_t* EA_RESTRICT pFormat, va_list arguments);
EASTDC_API int Vsnprintf(char32_t* EA_RESTRICT pDestination, size_t n, const char32_t* EA_RESTRICT pFormat, va_list arguments);

#if defined(EA_WCHAR_UNIQUE) && EA_WCHAR_UNIQUE
EASTDC_API int Vsnprintf(wchar_t* EA_RESTRICT pDestination, size_t n, const wchar_t* EA_RESTRICT pFormat, va_list arguments);
#endif

EASTDC_API int Sprintf(char* EA_RESTRICT pDestination, size_t pDestSize, const char* EA_RESTRICT pFormat, ...)
{
    va_list arguments;
    va_start(arguments, pFormat);

#if _MSC_VER
    auto result = vsprintf_s(pDestination, pDestSize, pFormat, arguments);

#else
    UNREFERENCED_PARAMETER(pDestSize);
    auto result = vsprintf_s(pDestination, pFormat, arguments);

#endif

    va_end(arguments);

    return result;
}

}} // namespace EA::StdC

#endif  // EASTDC_EASPRINTF_H
