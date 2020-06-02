// This file is a part of xray-ng engine
//

#include "sfx/sfx_subsystem.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
/**
 */
sfx_subsystem::sfx_subsystem(extension::context* ctx, memory::base_allocator& alloc)
    : extension::subsystem(ctx)
    , m_allocator(alloc)
{
    initialize_self();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
sfx_subsystem::~sfx_subsystem()
{
    destroy_self();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
bool sfx_subsystem::initialize_async(tasks::execution_context& ctx)
{
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_subsystem::tick_async(tasks::execution_context& ctx, float delta)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_subsystem::initialize_self()
{
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_subsystem::destroy_self()
{
}

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------