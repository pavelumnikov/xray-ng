// This file is a part of xray-ng engine
//

#pragma once

#include "extension/subsystem.h"
#include "sfx/sfx_api.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
class sfx_subsystem : public extension::subsystem
{
public:
    sfx_subsystem(extension::context* ctx, memory::base_allocator& alloc);
    virtual ~sfx_subsystem();
    virtual bool initialize_async(tasks::execution_context& ctx);
    virtual void tick_async(tasks::execution_context& ctx, sys::tick delta);
    api* get_sfx_api();

private:
    void initialize_self();
    void destroy_self();

    memory::base_allocator& m_allocator;
    api* m_sfx_api;
}; // class sfx_subsystem

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------