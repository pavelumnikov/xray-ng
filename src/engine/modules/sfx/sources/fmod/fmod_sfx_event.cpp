// This file is a part of xray-ng engine
//

#include "pch.h"
#include "fmod_sfx_event.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
/**
 */
fmod_sfx_event::fmod_sfx_event(fmod_event_descriptions& descriptions, utils::string_view name)
    : m_allocator { descriptions.get_allocator() }
    , m_event_instance { nullptr }
{
    FMOD::Studio::EventDescription* desc = descriptions.add_or_get_from_cache(name);
    desc->createInstance(&m_event_instance);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
fmod_sfx_event::~fmod_sfx_event()
{
    XR_DEBUG_ASSERTION_MSG(m_event_instance != nullptr, "invalid FMOD event instance");
    m_event_instance->release();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_sfx_event::start()
{
    XR_DEBUG_ASSERTION_MSG(m_event_instance != nullptr, "invalid FMOD event instance");
    m_event_instance->start();
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_sfx_event::stop_fadeout()
{
    XR_DEBUG_ASSERTION_MSG(m_event_instance != nullptr, "invalid FMOD event instance");
    m_event_instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_sfx_event::stop()
{
    XR_DEBUG_ASSERTION_MSG(m_event_instance != nullptr, "invalid FMOD event instance");
    m_event_instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------