// This file is a part of xray-ng engine
//

#include "rendering_module.h"
#include "rendering_api/render_device.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, modules)

//-----------------------------------------------------------------------------------------------------------
/**
 */
rendering_module::rendering_module(memory::base_allocator& alloc, GLFWwindow* window)
    : m_allocator { alloc }
    , m_render_device { nullptr }
    , m_window { window }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
rendering_module::~rendering_module()
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void rendering_module::intialize()
{
    rendering_api::render_init_result init_result;
    rendering_api::descs::renderer_desc render_desc {};
    // TODO: remove this to work with various HW and renders
    render_desc.api = rendering_api::render_api::vulkan;
    render_desc.enable_gpu_based_validation = false;
    render_desc.mode = rendering_api::gpu_mode::single;
    render_desc.target = rendering_api::shader_target::sm_5_1;
    m_render_device = rendering_api::initialize(m_allocator, render_desc, init_result);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void rendering_module::shutdown()
{
    XR_DEBUG_ASSERTION_MSG(m_render_device != nullptr, "render device must be initialized at this point");
    rendering_api::render_destroy_result destroy_result;
    rendering_api::shutdown(m_render_device, destroy_result);
}

XR_NAMESPACE_END(xr, game, modules)
//-----------------------------------------------------------------------------------------------------------