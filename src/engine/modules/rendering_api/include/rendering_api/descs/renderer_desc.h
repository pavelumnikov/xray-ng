// This file is a part of xray-ng engine
//

#pragma once

#include "rendering_api/common.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api, descs)

//-----------------------------------------------------------------------------------------------------------
struct renderer_desc
{
    render_api api;
    shader_target target;
    gpu_mode mode;

    // This results in new validation not possible during API calls on the CPU, 
    // by creating patched shaders that have validation added directly to the shader.
    // However, it can slow things down a lot, especially for applications with numerous PSOs.
    // Time to see the first render frame may take several minutes
    bool enable_gpu_based_validation;
}; // struct renderer_desc

XR_NAMESPACE_END(xr, rendering_api, descs)
//-----------------------------------------------------------------------------------------------------------
