// This file is a part of xray-ng engine
//

#pragma once

#include "fmod_device.h"
#include "fmod_event_descriptions.h"
#include "sfx/sfx_api.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx, fmod_impl)

//-----------------------------------------------------------------------------------------------------------
class fmod_sfx_api : public api
{
public:
    fmod_sfx_api(memory::base_allocator& alloc);
    virtual ~fmod_sfx_api() = default;

    virtual utils::shared_ptr<environment> create_environment() override;
    virtual void switch_to_environment(utils::shared_ptr<environment> const& env) override;

    fmod_device& get_device();

private:
    fmod_device m_device;
    fmod_event_descriptions m_event_descriptions;
}; // class fmod_sfx_api

//-----------------------------------------------------------------------------------------------------------
/**
 */
inline fmod_device& fmod_sfx_api::get_device()
{
    return m_device;
}

XR_NAMESPACE_END(xr, sfx, fmod_impl)
//-----------------------------------------------------------------------------------------------------------