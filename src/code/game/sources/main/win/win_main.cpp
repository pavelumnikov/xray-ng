// This file is a part of xray-ng engine
//

#include "win_main.h"
#include <Windows.h>

#define XRAY_IDI_APPLICATION MAKEINTRESOURCEW(32512)
#define XRAY_IDC_ARROW MAKEINTRESOURCEW(32512)

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, win)

//-----------------------------------------------------------------------------------------------------------

static XR_CONSTEXPR_CPP14_OR_CONST wchar_t kWindowName[] = L"xray-ng";
static XR_CONSTEXPR_CPP14_OR_CONST wchar_t kWindowClass[] = L"XRAYNG_CLASS";
static HWND kWindowHandle = nullptr;

//-----------------------------------------------------------------------------------------------------------
/**
 */
LRESULT CALLBACK static_wnd_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_KEYDOWN:
        {
            if(wParam == VK_ESCAPE)
            {
                if(MessageBoxW(hWnd, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
                {
                    DestroyWindow(hWnd);
                }
            }
            return 0;
        }

        case WM_DESTROY: // x button on top right corner of window was pressed
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool create_main_window(memory::base_allocator& allocator, uint32_t width, uint32_t height, bool fullscreen)
{
    WNDCLASSEXW wc;
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandleW(NULL);

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = static_wnd_proc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIconW(nullptr, XRAY_IDI_APPLICATION);
    wc.hCursor = LoadCursorW(nullptr, XRAY_IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = kWindowClass;
    wc.hIconSm = LoadIconW(nullptr, XRAY_IDI_APPLICATION);

    if(!RegisterClassExW(&wc))
    {
        MessageBoxW(nullptr, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    kWindowHandle = CreateWindowExW(0,
        kWindowClass,
        kWindowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if(!kWindowHandle)
    {
        MessageBoxW(nullptr, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void show_main_window()
{
    ShowWindow(kWindowHandle, SW_SHOWNA);
    UpdateWindow(kWindowHandle);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool run_main_loop()
{
    MSG msg;
    bool result = true;
    while(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if(msg.message == WM_QUIT)
        {
            result = false;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return result;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void destroy_main_window()
{
}

XR_NAMESPACE_END(xr, game, win)
//-----------------------------------------------------------------------------------------------------------
