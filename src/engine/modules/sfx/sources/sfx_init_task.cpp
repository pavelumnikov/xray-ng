// This file is a part of xray-ng engine
//

#include "pch.h"
#include "sfx_init_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, sfx)

//-----------------------------------------------------------------------------------------------------------
/**
 */
sfx_init_task::sfx_init_task(fmod_impl::fmod_device* device)
    : m_device { device }
{
    XR_DEBUG_ASSERTION(device != nullptr);
}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void sfx_init_task::operator()(tasks::execution_context& context)
{
    XR_DEBUG_ASSERTION_MSG(m_device != nullptr, "FMOD device is invalid");
    m_device->initialize();
}

XR_NAMESPACE_END(xr, sfx)
//-----------------------------------------------------------------------------------------------------------