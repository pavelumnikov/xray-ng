// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/types.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, rendering_api)

//-----------------------------------------------------------------------------------------------------------
enum class render_api : uint8_t
{
    d3d12,
    vulkan,
    metal,
    d3d12_xbox,
    d3d11 // fallback render type
}; // enum class render_api

//-----------------------------------------------------------------------------------------------------------
enum class shader_target : uint8_t
{
    sm_5_0, // D3D11-only
    sm_5_1, // D3D12 and never
    sm_6_0,
    sm_6_1,
    sm_6_2,
    sm_6_3 // TODO: required by Raytracing (add Raytracing)
}; // enum class shader_target

//-----------------------------------------------------------------------------------------------------------
enum class gpu_mode : uint8_t
{
    single,
    linked
}; // enum class gpu_mode

XR_NAMESPACE_END(xr, rendering_api)
//-----------------------------------------------------------------------------------------------------------