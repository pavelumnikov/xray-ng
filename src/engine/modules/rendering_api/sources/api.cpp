// This file is a part of xray-ng engine
//

#pragma once

#include "rendering_api/api.h"
#include "vk/vk_render_device.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
/**
 */
render_device* initialize(memory::base_allocator& alloc, descs::renderer_desc const& desc,
    render_init_result& result)
{
    vk::vk_render_device* render = XR_ALLOCATE_OBJECT_T(alloc, vk::vk_render_device, "Vulkan Render")(alloc);
    render->initialize(true);

    // TODO: add render device creation check
    result = render_init_result::success;
    return render;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool shutdown(render_device* device, render_destroy_result& result)
{
    memory::base_allocator& alloc = device->get_allocator();
    
    static_cast<base_render_device*>(device)->shutdown();
    XR_DEALLOCATE_MEMORY_T(alloc, device);

    // TODO: add render device destruction check
    result = render_destroy_result::success;
    return true;
}

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------