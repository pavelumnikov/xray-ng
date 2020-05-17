// This file is a part of xray-ng engine
//

#pragma once

#include "rendering_api/render_device.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
XR_DECLARE_SINGLE_INHERITED_INTERFACE(base_render_device, render_device)
{
    XR_PURE_VIRTUAL_DESTRUCTOR(base_render_device);
    virtual void initialize(bool enable_debug_layer) = 0;
    virtual void shutdown() = 0;
}; // XR_DECLARE_SINGLE_INHERITED_INTERFACE(base_render_device, render_device)

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------