// This file is a part of xray-ng engine
//

#include "debug_renderdoc.h"
#include "corlib/memory/allocator_macro.h"

#if XRAY_PLATFORM_WINDOWS
#include <Windows.h>
#include <psapi.h>
#endif // XRAY_PLATFORM_WINDOWS

#include <renderdoc/renderdoc_app.h>

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
namespace
{

//-----------------------------------------------------------------------------------------------------------
/**
 */
static bool find_module(const char* _name)
{
#if XRAY_PLATFORM_WINDOWS
    DWORD size;
    HANDLE process = GetCurrentProcess();
    BOOL result = EnumProcessModules(process, nullptr, 0, &size);

    if(result)
    {
        HMODULE* modules = XR_STACK_ALLOCATE_CAST_T(HMODULE, size);
        result = EnumProcessModules(process, modules, size, &size);

        if(result)
        {
            char module_name[MAX_PATH];
            for(uint32_t ii = 0, num = uint32_t(size / sizeof(HMODULE)); ii < num; ++ii)
            {
                result = GetModuleBaseNameA(process, modules[ii], module_name, eastl::size(module_name));
                if(result && !strcmpi(_name, module_name))
                    return true;
            }
        }
    }
#endif // BX_PLATFORM_WINDOWS

    XR_UNREFERENCED_PARAMETER(_name);
    return false;
}
} // anonymous namespace
//-----------------------------------------------------------------------------------------------------------

pRENDERDOC_GetAPI RENDERDOC_GetAPI;
static RENDERDOC_API_1_1_2* the_renderdoc = nullptr;
static pvoid the_renderdoc_dll = nullptr;

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool load_renderdoc()
{
    if(the_renderdoc_dll)
        return true;

    // Skip loading RenderDoc when IntelGPA is present to avoid RenderDoc crash.
    if(find_module(XRAY_PLATFORM_64BIT ? "shimloader64.dll" : "shimloader32.dll"))
        return false;

    pvoid renderdoc_dll = sys::dll_load(
#if XRAY_PLATFORM_WINDOWS
        "renderdoc.dll"
#else
        "./librenderdoc.so"
#endif // BX_PLATFORM_WINDOWS
    );

    if(renderdoc_dll)
    {
        RENDERDOC_GetAPI = sys::dll_require_typed_symbol<pRENDERDOC_GetAPI>(renderdoc_dll, "RENDERDOC_GetAPI");
        if(RENDERDOC_GetAPI && RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void**)&the_renderdoc))
        {
            RENDERDOC_InputButton capture_keys[] = { eRENDERDOC_Key_F11 };

            the_renderdoc->SetCaptureFilePathTemplate("temp/xray-ng");
            the_renderdoc->SetFocusToggleKeys(nullptr, 0);
            the_renderdoc->SetCaptureKeys(capture_keys, eastl::size(capture_keys));
            the_renderdoc->SetCaptureOptionU32(eRENDERDOC_Option_AllowVSync, 1);
            the_renderdoc->SetCaptureOptionU32(eRENDERDOC_Option_SaveAllInitials, 1);
            the_renderdoc->MaskOverlayBits(eRENDERDOC_Overlay_None, eRENDERDOC_Overlay_None);

            the_renderdoc_dll = renderdoc_dll;
        }
        else
        {
            sys::dll_unload(renderdoc_dll);
            renderdoc_dll = nullptr;
        }
    }

    return renderdoc_dll != nullptr;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void unload_renderdoc()
{
    // BK - Once RenderDoc is loaded there shouldn't be calls
    // to shutdown or unload RenderDoc DLL.
    // https://github.com/bkaradzic/bgfx/issues/1192

    if(the_renderdoc)
    {
        // the_renderdoc->Shutdown();
    }

    if(the_renderdoc_dll)
    {
        // sys::dll_unload(renderdoc_dll);
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void renderdoc_trigger_capture()
{
    if(the_renderdoc)
        the_renderdoc->TriggerCapture();
}

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------
