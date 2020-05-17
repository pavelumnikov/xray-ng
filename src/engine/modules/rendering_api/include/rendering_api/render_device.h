// This file is a part of xray-ng engine
//

#pragma once

#include "rendering_api/handle.h"
#include "corlib/memory/memory_allocator_base.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
XR_DECLARE_INTERFACE(render_device)
{
    XR_PURE_VIRTUAL_DESTRUCTOR(render_device);
    virtual memory::base_allocator& get_allocator() = 0;
};

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------