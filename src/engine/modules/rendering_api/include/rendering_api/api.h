// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "rendering_api/descs/renderer_desc.h"
#include "rendering_api/render_device.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
enum class render_init_result
{
    success,
    not_initialized
}; // enum class render_init_result

//-----------------------------------------------------------------------------------------------------------
enum class render_destroy_result
{
    success,
    resources_leaked
}; // enum class render_destroy_result

//-----------------------------------------------------------------------------------------------------------
/**
 */
render_device* initialize(memory::base_allocator& alloc, descs::renderer_desc const& desc,
    render_init_result& result);

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool shutdown(render_device* device, render_destroy_result& result);

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------