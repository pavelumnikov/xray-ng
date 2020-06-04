// This file is a part of xray-ng engine
//

#include "tick_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, extension, task)

//-----------------------------------------------------------------------------------------------------------
/**
 */
tick_task::tick_task(utils::shared_ptr<subsystem>& system, float delta)
    : m_system { system }
    , m_delta { delta }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void tick_task::operator()(tasks::execution_context& context)
{
    m_system->tick_async(context, m_delta);
}

XR_NAMESPACE_END(xr, extension, task)
//-----------------------------------------------------------------------------------------------------------