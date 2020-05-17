// This file is a part of xray-ng engine
//

#pragma once

#define VK_NO_PROTOTYPES // we define them ourself

#if XRAY_PLATFORM_WINDOWS
// When VK_USE_PLATFORM_WIN32_KHR is defined, instead of including vulkan.h directly,
// we include individual parts of the SDK.
// This is necessary to avoid including <windows.h> which is very heavy - it takes 200ms to
// parse without WIN32_LEAN_AND_MEAN and 100ms to parse with it. vulkan_win32.h only needs
// a few symbols that are easy to redefine ourselves.s
typedef unsigned long DWORD;
typedef const wchar_t* LPCWSTR;
typedef void* HANDLE;
typedef struct HINSTANCE__* HINSTANCE;
typedef struct HWND__* HWND;
typedef struct HMONITOR__* HMONITOR;
typedef struct _SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_win32.h>

#else
#include <vulkan/vulkan.h>

#endif // XRAY_PLATFORM_WINDOWS

#define VK_CHECK_RESULT(result) (result == VK_SUCCESS)