// This file is a part of xray-ng engine
//

#pragma once

#include "extension/subsystem.h"
#include "async_io/file_api.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, async_io)

//-----------------------------------------------------------------------------------------------------------
class async_io_subsystem : public extension::subsystem
{
public:
    async_io_subsystem(extension::context* ctx, memory::base_allocator& alloc);
    virtual ~async_io_subsystem();
    virtual bool initialize_async(tasks::execution_context& ctx);
    virtual void tick_async(tasks::execution_context& ctx, sys::tick delta);
    file_api& get_file_api();

private:
    void initialize_self();
    void destroy_self();

    memory::base_allocator& m_allocator;
    file_api* m_file_api;
}; // class async_io_subsystem

XR_NAMESPACE_END(xr, async_io)
//-----------------------------------------------------------------------------------------------------------