// This file is a part of xray-ng engine
//

#include "sfx/sfx_subsystem.h"
#include "fmod/fmod_sfx_api.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
/**
 */
sfx_subsystem::sfx_subsystem(extension::context* ctx, memory::base_allocator& alloc)
    : extension::subsystem { ctx }
    , m_allocator { alloc }
    , m_sfx_api { nullptr }
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
    fmod_impl::fmod_sfx_api* api = static_cast<fmod_impl::fmod_sfx_api*>(m_sfx_api);
    fmod_impl::fmod_device* device = &api->get_device();

    device->initialize();
    return true;
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_subsystem::tick_async(tasks::execution_context& ctx, sys::tick delta)
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_subsystem::initialize_self()
{
    m_sfx_api = XR_ALLOCATE_OBJECT_T(m_allocator, fmod_impl::fmod_sfx_api, "FMOD SFX API")(m_allocator);
    XR_DEBUG_ASSERTION_MSG(m_sfx_api != nullptr, "failed to initialize FMOD SFX API");
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_subsystem::destroy_self()
{
    XR_DEALLOCATE_MEMORY_T(m_allocator, m_sfx_api);
}

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------