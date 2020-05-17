// This file is a part of xray-ng engine
//

#pragma once

#include "corlib/memory/memory_allocator_base.h"
#include "rendering_api/api.h"
#include "GLFW/glfw3.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, modules)

//-----------------------------------------------------------------------------------------------------------
class rendering_module
{
public:
    rendering_module(memory::base_allocator& alloc, GLFWwindow* window);
    ~rendering_module();

    XR_DECLARE_DELETE_COPY_ASSIGNMENT(rendering_module);
    XR_DECLARE_DELETE_MOVE_ASSIGNMENT(rendering_module);

    void intialize();
	void shutdown();

private:
    memory::base_allocator& m_allocator;
    rendering_api::render_device* m_render_device;
    GLFWwindow* m_window;
}; // class rendering_module

XR_NAMESPACE_END(xr, game, modules)
//-----------------------------------------------------------------------------------------------------------