// This file is a part of xray-ng engine
//

#include "application.h"
#include "corlib/threading/interlocked.h"
#include "corlib/memory/memory_mt_arena_allocator.h"
#include "corlib/memory/memory_st_arena_allocator.h"
#include "corlib/memory/memory_crt_allocator.h"
#include "corlib/memory/uninitialized_reference.h"
#include "corlib/sys/thread.h"
#include "corlib/sys/chrono.h"
#include "../config.h"
#include "../constants.h"

#if defined(XRAY_PLATFORM_WINDOWS)
#include <Windows.h>
#endif

using namespace xr;

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on Nvidia Optimus systems
// with up-to-date drivers
//
__declspec(dllexport) DWORD NvOptimusEnablement = 1;

// Executables (but not DLLs) exporting this symbol with this value will be
// automatically directed to the high-performance GPU on AMD PowerXpress systems
// with up-to-date drivers
//
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

//-----------------------------------------------------------------------------------------------------------
/**
 */
int main(int argc, char** argv)
{
    memory::crt_allocator misc_allocator {};
    game::main::early_initialize_application(misc_allocator);

    memory::mt_arena_allocator io_system_allocator {};
    io_system_allocator.initialize(XR_MEGABYTES_TO_BYTES(64), XR_MEGABYTES_TO_BYTES(16));

    memory::mt_arena_allocator gfx_subsystem_allocator {};
    gfx_subsystem_allocator.initialize(XR_MEGABYTES_TO_BYTES(256), XR_MEGABYTES_TO_BYTES(16));

    memory::mt_arena_allocator sfx_subsystem_allocator {};
    sfx_subsystem_allocator.initialize(XR_MEGABYTES_TO_BYTES(64), XR_MEGABYTES_TO_BYTES(16));

    game::main::initialize_application_desc desc
    {
        misc_allocator,
        io_system_allocator,
        gfx_subsystem_allocator,
        sfx_subsystem_allocator
    };

    game::main::initialize_application(desc);

    while(true)
    {
        if(!game::main::run_application())
            break;
    }

    game::main::shutdown_application();
    return 0;
}

#if defined(XRAY_PLATFORM_WINDOWS)
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    XR_UNREFERENCED_PARAMETER(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return main(__argc, __argv);
}
#endif