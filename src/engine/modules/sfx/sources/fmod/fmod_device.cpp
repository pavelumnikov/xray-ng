// This file is a part of xray-ng engine
//

#include "pch.h"
#include "fmod_device.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
/**
 */
fmod_device::fmod_device(memory::base_allocator& alloc)
    : m_allocator { alloc }
    , m_system { nullptr }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_device::initialize()
{
    FMOD_RESULT result = FMOD::Studio::System::create(&m_system);
    if(!result)
    {
        // TODO: error handling
    }

    XR_DEBUG_ASSERTION_MSG(m_system != nullptr, "FMOD Studio system not initialized properly");

    XR_CONSTEXPR_CPP14_OR_CONST FMOD_STUDIO_INITFLAGS studio_init_flags = FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE;
    XR_CONSTEXPR_CPP14_OR_CONST FMOD_INITFLAGS init_flags = FMOD_INIT_CHANNEL_LOWPASS | FMOD_INIT_GEOMETRY_USECLOSEST;

    result = m_system->initialize(512, studio_init_flags, init_flags, nullptr);
    if(!result)
    {
        // TODO: error handling
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_device::destroy()
{
    XR_DEBUG_ASSERTION_MSG(m_system != nullptr, "FMOD Studio system not initialized");
    m_system->release();
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------