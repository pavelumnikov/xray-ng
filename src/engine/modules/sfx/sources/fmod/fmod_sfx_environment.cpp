// This file is a part of xray-ng engine
//

#include "pch.h"
#include "fmod_sfx_environment.h"
#include "fmod_sfx_event.h"
#include "corlib/memory/allocator_macro.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
/**
 */
static void free_fmod_event(event* ev)
{
    XR_DEBUG_ASSERTION_MSG(ev != nullptr, "invalid SFX Event");
    fmod_sfx_event* fmod_event = static_cast<fmod_sfx_event*>(ev);
    memory::base_allocator& alloc = fmod_event->get_allocator();
    XR_DEALLOCATE_MEMORY_T(alloc, fmod_event);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
fmod_sfx_environment::fmod_sfx_environment(fmod_device& device, fmod_event_descriptions& event_descriptions)
    : m_device { device }
    , m_event_descriptions { event_descriptions }
    , m_proxy_allocator { device.get_allocator() }
    , m_vca_bus { nullptr }
    , m_enviroment_events { m_proxy_allocator }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
utils::shared_ptr<event> fmod_sfx_environment::create_event(utils::string_view name)
{
    memory::base_allocator& alloc = m_device.get_allocator();
    event* result = XR_ALLOCATE_OBJECT_T(alloc, fmod_sfx_event, "FMOD SFX Event")(m_event_descriptions, name);
    XR_DEBUG_ASSERTION_MSG(result != nullptr, "failed to create SFX Event");
    return utils::shared_ptr<event>(result, free_fmod_event);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void fmod_sfx_environment::stop_all_events()
{
    for(auto current = m_enviroment_events.begin(); current != m_enviroment_events.end(); ++current)
    {
        (*current)->stop();
    }
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
uint32_t fmod_sfx_environment::current_playing_events()
{
    return 0; // TODO: implement this
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------