// This file is a part of xray-ng engine
//

#include "rendering_subsystem_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, task)

//-----------------------------------------------------------------------------------------------------------
/**
*/
rendering_subsystem_task::rendering_subsystem_task(modules::rendering_module& rm)
    : m_rendering_module { rm }
{}

//-----------------------------------------------------------------------------------------------------------
/**
*/
void rendering_subsystem_task::operator()(tasks::execution_context& context)
{}

XR_NAMESPACE_END(xr, game, task)
//-----------------------------------------------------------------------------------------------------------