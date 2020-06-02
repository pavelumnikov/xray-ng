// This file is a part of xray-ng engine
//

#pragma once

#include "sfx/sfx_event.h"
#include "fmod_event_descriptions.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
class fmod_sfx_event : public event
{
public:
    fmod_sfx_event(fmod_event_descriptions& descriptions, utils::string_view name);
    virtual ~fmod_sfx_event();

    virtual void start() override;
    virtual void stop_fadeout() override;
    virtual void stop() override;

    memory::base_allocator& get_allocator();

private:
    memory::base_allocator& m_allocator; // for releasing
    FMOD::Studio::EventInstance* m_event_instance;
}; // class fmod_sfx_event

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline memory::base_allocator& fmod_sfx_event::get_allocator()
{
    return m_allocator;
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------