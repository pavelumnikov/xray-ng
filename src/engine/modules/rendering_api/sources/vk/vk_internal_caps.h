// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/utils/vector.h"
#include "vk_include.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api, vk)

//-----------------------------------------------------------------------------------------------------------
struct vk_internal_caps
{
    bool renderdoc_layer_enabled { false };
    bool dedicated_allocation_extension { false };
    bool external_memory_extension { false };
    bool draw_indirect_count_extension { false };
    bool device_group_creation_extension { false };
    bool descriptor_indexing_extension { false };
    bool amd_draw_indirect_count_extension { false };
    bool amd_gcn_shader_extension { false };
    bool nv_raytracing_extension { false }; // TODO: support this
}; // struct vk_internal_caps

XR_NAMESPACE_END(xr, rendering_api, vk)
//-----------------------------------------------------------------------------------------------------------
