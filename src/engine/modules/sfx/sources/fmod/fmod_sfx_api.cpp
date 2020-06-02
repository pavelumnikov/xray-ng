// This file is a part of xray-ng engine
//

#include "pch.h"
#include "fmod_sfx_api.h"
#include "fmod_sfx_environment.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
/**
 */
static void free_fmod_environment(environment* env)
{
    XR_DEBUG_ASSERTION_MSG(env != nullptr, "invalid SFX Event");
    fmod_sfx_environment* fmod_env = static_cast<fmod_sfx_environment*>(env);
    memory::base_allocator& alloc = fmod_env->get_allocator();
    XR_DEALLOCATE_MEMORY_T(alloc, fmod_env);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
fmod_sfx_api::fmod_sfx_api(memory::base_allocator& alloc)
    : m_device { alloc }
    , m_event_descriptions { alloc, m_device.get_fmod_system() }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
utils::shared_ptr<environment> fmod_sfx_api::create_environment()
{
    memory::base_allocator& alloc = m_device.get_allocator();
    environment* result = XR_ALLOCATE_OBJECT_T(alloc, fmod_sfx_environment, "FMOD SFX Environment")
        (m_device, m_event_descriptions);

    XR_DEBUG_ASSERTION_MSG(result != nullptr, "failed to create SFX Environment");
    return utils::shared_ptr<environment>(result, free_fmod_environment);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_sfx_api::switch_to_environment(utils::shared_ptr<environment> const& env)
{
    // TODO: implement me
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------