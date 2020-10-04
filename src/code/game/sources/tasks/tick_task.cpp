// This file is a part of xray-ng engine
//

#include "tick_task.h"

//-----------------------------------------------------------------------------------------------------------
XR_NAMESPACE_BEGIN(xr, game, task)

//-----------------------------------------------------------------------------------------------------------
/**
 */
tick_task::tick_task(engine::extension::context& ctx, sys::tick delta)
    : m_extension_context { ctx }
    , m_delta { delta }
{}

//-----------------------------------------------------------------------------------------------------------
/**
 */
void tick_task::operator()(tasks::execution_context& context)
{
    m_extension_context.tick_async(context, m_delta);
}

XR_NAMESPACE_END(xr, game, task)
//-----------------------------------------------------------------------------------------------------------