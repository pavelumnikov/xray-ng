// This file is a part of xray-ng engine
//

#pragma once

#include "sfx/sfx_environment.h"
#include "fmod_device.h"
#include "fmod_event_descriptions.h"
#include "corlib/utils/vector.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
class fmod_sfx_environment : public environment
{
public:
    fmod_sfx_environment(fmod_device& device, fmod_event_descriptions& event_descriptions);
    virtual ~fmod_sfx_environment() = default;

    virtual utils::shared_ptr<event> create_event(utils::string_view name) override;
    virtual void stop_all_events() override;
    virtual uint32_t current_playing_events() override;

    memory::base_allocator& get_allocator();

private:
    fmod_device& m_device;
    fmod_event_descriptions& m_event_descriptions;
    memory::proxy::eastl_proxy_allocator m_proxy_allocator;
    FMOD::Studio::VCA* m_vca_bus;
    utils::vector<utils::shared_ptr<event>> m_enviroment_events;
}; // class fmod_sfx_environment

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline memory::base_allocator& fmod_sfx_environment::get_allocator()
{
    return m_proxy_allocator.get_base_allocator();
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------