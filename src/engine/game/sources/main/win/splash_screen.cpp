// This file is a part of xray-ng engine
//

#include "splash_screen.h"
#include "corlib/sys/thread.h"
#include "resource.h"
#include <Windows.h>

//-----------------------------------------------------------------------------------------------------------
namespace xr::game::main::details
{

struct splash_screen_context final
{
    HWND hwnd { nullptr };

    sys::thread_handle worker_thread 
    {
        sys::unknown_thread_handle 
    };

    bool handle_valid { false };
};

//-----------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK window_procedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    XR_UNREFERENCED_PARAMETER(wp, lp, hwnd);

    if(msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return FALSE;
    }

    return 0;
};

//-----------------------------------------------------------------------------------------------------------
uint32_t on_splash_screen(void* const arg) noexcept
{
    XR_DEBUG_ASSERTION(arg != nullptr);
    auto const splash_screen = static_cast<splash_screen_context*>(arg);

    constexpr char file_name[19] = { "LauncherSplash.bmp" };
    auto const bitmap_handle = static_cast<HBITMAP>(::LoadImageA(
        ::GetModuleHandleA(nullptr), file_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

    if(!bitmap_handle)
    {
        //LOG_ERROR("cannot create bitmap for splash screen from file \"%s\" ", file_name.c_str());
        return 1;
    }

    BITMAP bm; // get bitmap data
    GetObjectA(bitmap_handle, sizeof(BITMAP), &bm);

    auto const module = ::GetModuleHandleA(nullptr);
    splash_screen->hwnd = ::CreateDialogA(module, MAKEINTRESOURCE(IDD_SPLASH_SCREEN), nullptr, window_procedure);
    ::SetDlgItemTextA(splash_screen->hwnd, IDC_FINGER_PRINT, "Launcher");

    auto const picture_control = ::GetDlgItem(splash_screen->hwnd, IDC_SPLASH_SCREEN);
    if(!picture_control)
    {
        PostMessageA(splash_screen->hwnd, WM_DESTROY, 0, 0);
        return 1;
    }

    // ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/shellcc/platform/commctls/staticcontrols/staticcontrolreference/staticcontrolmessages/stm_setimage.htm
    auto const previous_bitmap = reinterpret_cast<HBITMAP>(::SendMessageA(picture_control,
        STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(bitmap_handle)));

    assert((!previous_bitmap) && "do not assign image to splash screen in design mode");
    if(previous_bitmap)
        ::DeleteObject(previous_bitmap);

#ifndef MASTER_GOLD
    static auto const hwnd_insert_after = HWND_NOTOPMOST;
#else
    static auto const hwnd_insert_after = HWND_TOPMOST;
#endif

    SetWindowPos(splash_screen->hwnd, hwnd_insert_after,
        (GetSystemMetrics(SM_CXSCREEN) - bm.bmWidth) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - bm.bmHeight) / 2,
        bm.bmWidth, bm.bmHeight, SWP_SHOWWINDOW);

    SetWindowPos(splash_screen->hwnd, hwnd_insert_after,
        0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    UpdateWindow(splash_screen->hwnd);

    MSG message;
    for(;;)
    {
        auto const result = !!::GetMessageA(&message, nullptr, 0, 0);
        if(!result)
            break;

        // At least we must do one cycle in this loop before
        // handle can become valid
        if(!splash_screen->handle_valid)
            splash_screen->handle_valid = true;

        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    // in some cases this could be done after STM_SETIMAGE
    // but it is more safe to do this here always

    DeleteObject(bitmap_handle);
    return 0;
}

} // namespace xr::game::main::details

//-----------------------------------------------------------------------------------------------------------
namespace xr::game::main
{

// Global Variables:
details::splash_screen_context g_splash_screen;

//-----------------------------------------------------------------------------------------------------------
/**
*/
void create_splash_screen(memory::base_allocator& allocator)
{
    g_splash_screen.worker_thread = sys::spawn_thread(
        details::on_splash_screen, &g_splash_screen, L"splash screen",
        sys::thread_priority::medium, 32_kb, 0);

    XR_DEBUG_ASSERTION_MSG(
        g_splash_screen.worker_thread != sys::unknown_thread_handle,
        "Splash screen hasn't been created");
}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void destroy_splash_screen()
{
    while(!g_splash_screen.handle_valid)
        SwitchToThread(); // do switching until handle becomes valid

    PostMessageA(g_splash_screen.hwnd, WM_DESTROY, 0, 0);
    bool volatile const result = sys::wait_threads(
        &g_splash_screen.worker_thread, 1);

    (result); // Nothing to do with this
    g_splash_screen.worker_thread = sys::unknown_thread_handle;
}

} // namespace xr::paranoia
//-----------------------------------------------------------------------------------------------------------