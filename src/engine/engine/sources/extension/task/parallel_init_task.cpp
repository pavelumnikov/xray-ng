// This file is a part of xray-ng engine
//

#include "pch.h"
#include "parallel_init_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, engine, extension, task)

//-----------------------------------------------------------------------------------------------------------
/**
 */
parallel_init_task::parallel_init_task(utils::shared_ptr<subsystem>& system)
    : m_system { system }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void parallel_init_task::operator()(tasks::execution_context& context)
{
    m_system->initialize_async(context);
}

XR_NAMESPACE_END(xr, engine, extension, task)
//-----------------------------------------------------------------------------------------------------------