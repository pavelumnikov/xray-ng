// This file is a part of xray-ng engine
//

#include "pch.h"
#include "fmod_event_descriptions.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
/**
 */
fmod_event_descriptions::fmod_event_descriptions(memory::base_allocator& alloc, FMOD::Studio::System* system)
    : m_proxy_allocator { alloc }
    , m_fmod_system { system }
    , m_descriptions { m_proxy_allocator }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
FMOD::Studio::EventDescription* fmod_event_descriptions::add_or_get_from_cache(utils::string_view name)
{
    XR_DEBUG_ASSERTION_MSG(m_fmod_system != nullptr, "FMOD System not initialized");
    auto end = m_descriptions.end();
    auto current = m_descriptions.find(name.data());

    FMOD::Studio::EventDescription* event_description;
    if(current == end)
        m_fmod_system->getEvent(name.data(), &event_description);
    else
        event_description = current->second;

    XR_DEBUG_ASSERTION_MSG(event_description != nullptr, "invalid FMOD Event Description");
    return event_description;
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------