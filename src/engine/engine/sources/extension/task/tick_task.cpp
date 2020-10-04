// This file is a part of xray-ng engine
//

#include "pch.h"
#include "tick_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, extension, task)

//-----------------------------------------------------------------------------------------------------------
/**
 */
tick_task::tick_task(utils::shared_ptr<subsystem>& system, sys::tick delta)
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

XR_NAMESPACE_END(xr, engine, extension, task)
//-----------------------------------------------------------------------------------------------------------