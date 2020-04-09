// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

#ifdef __clang__
#   define MINIMAL_WINDOWS_API CORE_API
#else
#   define MINIMAL_WINDOWS_API extern "C" __declspec(dllimport)
#endif

// Use strongly typed handles
#ifndef STRICT
#   define STRICT
#endif

// With STRICT enabled, handles are implemented as opaque struct pointers. We can prototype these structs here and 
// typedef them under the Windows namespace below. Since typedefs are only aliases rather than types in their own 
// right, this allows using handles from the Windows:: namespace interchangably with their native definition.
struct HINSTANCE__;
struct HWND__;
struct HKEY__;
struct HDC__;
struct HICON__;

// Other types
struct tagPROCESSENTRY32W;
struct _EXCEPTION_POINTERS;
struct _OVERLAPPED;
struct _RTL_CRITICAL_SECTION;
struct _RTL_CONDITION_VARIABLE;
union _LARGE_INTEGER;

// Global constants
#define WINDOWS_MAX_PATH 260
#define WINDOWS_PF_COMPARE_EXCHANGE128	14

// Standard calling convention for Win32 functions
#define WINAPI __stdcall

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

namespace xr::sys::win
{

// Typedefs for basic Windows types
typedef int32_t BOOL;
typedef unsigned long DWORD;
typedef DWORD* LPDWORD;
typedef long LONG;
typedef long* LPLONG;
typedef int64_t LONGLONG;
typedef LONGLONG* LPLONGLONG;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef const wchar_t* LPCTSTR;

typedef unsigned char BYTE;
typedef BYTE BOOLEAN;

// Typedefs for standard handles
typedef void* HANDLE;
typedef HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
typedef HWND__* HWND;
typedef HKEY__* HKEY;
typedef HDC__* HDC;
typedef HICON__* HICON;
typedef HICON__* HCURSOR;

// Other typedefs
typedef tagPROCESSENTRY32W PROCESSENTRY32;
typedef _EXCEPTION_POINTERS* LPEXCEPTION_POINTERS;
typedef _RTL_CRITICAL_SECTION* LPCRITICAL_SECTION;

typedef struct _RTL_CONDITION_VARIABLE
{
    void* Ptr;
} RTL_CONDITION_VARIABLE, * PRTL_CONDITION_VARIABLE;
typedef _RTL_CONDITION_VARIABLE* PCONDITION_VARIABLE;

typedef _OVERLAPPED* LPOVERLAPPED;
typedef _LARGE_INTEGER* LPLARGE_INTEGER;

typedef struct _RTL_SRWLOCK
{
    void* Ptr;
} RTL_SRWLOCK, *PRTL_SRWLOCK;

typedef RTL_SRWLOCK SRWLOCK, *PSRWLOCK;

// Constants
static const BOOL TRUE = 1;
static const BOOL FALSE = 0;

} // namespace xr::sys::win
